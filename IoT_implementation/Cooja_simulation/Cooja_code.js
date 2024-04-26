while (true) {
    
    YIELD_THEN_WAIT_UNTIL(msg.equals("Get motes position"));

    allm = sim.getMotes();
   
    for(var i = 0; i < allm.length; i++){
        var x = allm[i].getInterfaces().getPosition().getXCoordinate().toFixed(4);
        var y = allm[i].getInterfaces().getPosition().getYCoordinate().toFixed(4);    
        write(allm[i], x.toString()+","+y.toString()); 
    }
}