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
        Comms c = new Comms("Comm Thread");
        c.start();
        MapInterface.startMap();
        MapInterface.setComm(c);
    }
}
