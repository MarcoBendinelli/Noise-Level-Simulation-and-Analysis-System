package it.polimi.middleware.spark;

import it.polimi.middleware.spark.utils.LogUtils;
import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Row;
import org.apache.spark.sql.SparkSession;
import org.apache.spark.sql.expressions.Window;
import org.apache.spark.sql.expressions.WindowSpec;
import org.apache.spark.sql.types.DataTypes;
import org.apache.spark.sql.types.StructField;
import org.apache.spark.sql.types.StructType;
import java.util.ArrayList;
import java.util.List;

import static org.apache.spark.sql.functions.*;


public class DataAnalysis {

    private static final String csvName = "results.csv";

    private static final boolean useCache = true;
    private static final int nOfPoints = 10;
    private static final int maxNoise = 99;

    public static void main(String[] args) {
        LogUtils.setLogLevel();

        final String master = args.length > 0 ? args[0] : "local[4]";
        final String filePath = args.length > 1 ? args[1] : "../Back-end/In-Out_files/";
        final String appName = useCache ? "SparkWithCache" : "SparkNoCache";

        final SparkSession spark = SparkSession
                .builder()
                .master(master)
                .appName(appName)
                //.config("spark.sql.shuffle.partitions", 8)
                //.config("spark.executor.cores",2)
                .getOrCreate();

        final List<StructField> mySchemaFields = new ArrayList<>();
        mySchemaFields.add(DataTypes.createStructField("long", DataTypes.DoubleType, true));
        mySchemaFields.add(DataTypes.createStructField("lat", DataTypes.DoubleType, true));
        mySchemaFields.add(DataTypes.createStructField("noise", DataTypes.StringType, true));
        mySchemaFields.add(DataTypes.createStructField("point", DataTypes.StringType, true));
        mySchemaFields.add(DataTypes.createStructField("time", DataTypes.TimestampType, true));
        final StructType mySchema = DataTypes.createStructType(mySchemaFields);

        long startTime = System.nanoTime();

        //todo add to path "files/es1/" to use in intellij
        final Dataset<Row> data = spark
                .read()
                .option("header", "false")
                .option("delimiter", ",")
                .schema(mySchema)
                .csv(filePath+ csvName);

        long endTime_loading = System.nanoTime();

        final Dataset<Row> computedAvg = data.withColumn("splitted", split(col("noise"), "-"))//split array of noises
                .select(col("point"), col("time"), expr("AGGREGATE(splitted,cast(0 as double), (acc, x) -> acc + x,acc -> acc / size(`splitted`))")
                        .alias("value"));//get avg of array



        if (useCache) {
            computedAvg.cache();
        }

        final Dataset<Row> lastHourValues = computedAvg
                .where("time>now() - interval 1 hour");

        if(useCache){
            lastHourValues.cache();
        }

        /*
        final Dataset<Row> lastHour = computedAvg
                .select(col("point"),col("time"),
                        avg("value").over(Window.partitionBy("point")
                                .orderBy(col("time").cast("timestamp").cast("long"))
                                .rangeBetween(-3600, 0)).as("hourAvg"))
                .orderBy( "point", "time")
                ;
        lastHour.show();

        final Dataset<Row> lastDay = computedAvg
                .select(col("point"),col("time"),
                        avg("value").over(Window.partitionBy("point")
                                .orderBy(col("time").cast("timestamp").cast("long"))
                                .rangeBetween(-86400, 0)).as("dayAvg"))
                .orderBy( "point", "time")
        ;
        lastDay.show();


        final Dataset<Row> lastWeek = computedAvg
                .select(col("point"),col("time"),
                        avg("value").over(Window.partitionBy("point")
                                .orderBy(col("time").cast("timestamp").cast("long"))
                                .rangeBetween(-604800, 0)).as("weekAvg"))
                .orderBy( "point", "time")
                ;
        lastWeek.show();

*/







        //todo start of query 1
        final Dataset<Row> hourly = lastHourValues
                .groupBy("point").agg(avg("value").alias("last hour avg"));

        final Dataset<Row> daily = computedAvg
                .where("time>"+lit(date_sub(current_date(),1)))
                .groupBy("point").agg(avg("value").alias("last day avg"));

        final Dataset<Row> weekly = computedAvg
                .where("time>"+lit(date_sub(current_date(),6)))
                .groupBy("point").agg(avg("value").alias("last week avg"));


        //todo start of query 2
        final WindowSpec w2 = Window.partitionBy("point").orderBy(col("value").desc());
        final Dataset<Row> q2 = lastHourValues
                .withColumn("order",row_number().over(w2))
                .filter(col("order").equalTo(1))
                .orderBy(col("value").desc())
                .select("point","time","value")
                .limit(nOfPoints);

        //todo start of query 3
        final WindowSpec w3 = Window.partitionBy("point");
        final Dataset<Row> onlyOngoingStreaks = computedAvg.withColumn("seq", row_number().over(w3.orderBy(col("time").desc())))
                .filter(col("value").$less(maxNoise))
                .withColumn("new seq", row_number().over(w3.orderBy(col("time").desc())))
                .filter(col("seq").equalTo(col("new seq")));

        if (useCache) {
            onlyOngoingStreaks.cache();
        }

        //fixme is there a better way for this?
        final Dataset<Row> oldest = onlyOngoingStreaks.agg(min("time").as("time"));
        final Dataset<Row> q3 = onlyOngoingStreaks.join(oldest, "time")
                .select("point", "time","value")
                .withColumn("streak(sec)", current_timestamp().cast("long").minus(col("time").cast("long")))
                .dropDuplicates("point");//drop duplicates in case of two reading on the same time(might be useless)



        hourly.show();
        daily.show();
        weekly.show();
        long endTime_q1 = System.nanoTime();

        q2.show();
        long endTime_q2 = System.nanoTime();

        q3.show();
        long endTime_q3 = System.nanoTime();

        long endTime_output = System.nanoTime();


        System.out.println(
                "Loading time: " + (endTime_loading - startTime)/1000000 + "\n" +
                        "Q1 execution time: " + (endTime_q1 - endTime_loading)/1000000 + "\n" +
                        "Q2 execution time: " + (endTime_q2 - endTime_q1)/1000000 + "\n" +
                        "Q3 execution time: " + (endTime_q3 - endTime_q2)/1000000 + "\n" +
                        "Total time: " + (endTime_output - startTime)/1000000);


        spark.close();
    }
}