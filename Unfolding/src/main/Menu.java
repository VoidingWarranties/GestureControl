/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package main;

import java.io.IOException;

/**
 *
 * @author sheedb
 */

public class Menu{
    public static void main(String args[]) throws IOException{
        boolean valid = false;
        Comms c = new Comms("Comm Thread");
        c.start();
        System.out.println("Swipe left for Map, swipe right for 2048");
        while(!valid){
            while(!c.getNew());
            if(c.getInstruction().equals("Left")){
                MapInterface.startMap();
                MapInterface.setComm(c);
                valid = true;
            }
            else if(c.getInstruction().equals("Right")){
                c.setNew(false);
                com.bulenkov.game2048.Game2048.setComm(c);
                com.bulenkov.game2048.Game2048.main(null);
                valid = true;
            }
            c.setNew(false);
        }
    }
}
