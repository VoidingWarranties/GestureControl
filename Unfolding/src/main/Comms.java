/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package main;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author sheedb
 */
public class Comms implements Runnable{
    private boolean goToMenu = false;
    private Thread t;
    private String threadName;
    private BufferedReader in;
    private String inputLine;
    private ServerSocket serverSocket = null;
    private Socket clientSocket = null;
    private int portNumber = 4;
    private boolean newInstruction = false;
    private String instruction = "";
    
    Comms(String name){
        threadName = name;
        newInstruction = false;
        instruction = "";
    }
    
    public synchronized String getInstruction(){
        return instruction;
    }
    
    public synchronized boolean toMenu(){
        return goToMenu;
    }
    
    public synchronized boolean getNew(){
        return newInstruction;
    }
    
    public synchronized void setNew(boolean r){
        newInstruction = r;
    }
   
    public void run(){
        try {
            serverSocket = new ServerSocket(portNumber);
        } catch (IOException ex) {
            Logger.getLogger(Comms.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            clientSocket = serverSocket.accept();
        } catch (IOException ex) {
            Logger.getLogger(Comms.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        try {
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        } catch (IOException ex) {
            Logger.getLogger(Comms.class.getName()).log(Level.SEVERE, null, ex);
        }
        while(!goToMenu){
            try {
                inputLine = in.readLine();
            } catch (IOException ex) {
                Logger.getLogger(Comms.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            if(inputLine == null){
                break;
            }
            else{
                if(!newInstruction){
                    newInstruction = true;
                    instruction = inputLine;
                }
            }
        }
    }
    
    public void start() throws IOException{
      if (t == null)
      {
         t = new Thread (this, threadName);
         t.start ();
      }
    }
}
