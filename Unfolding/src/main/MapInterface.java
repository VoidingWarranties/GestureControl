/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package main;

/**
 *
 * @author sheedb
 */
import processing.core.PApplet;
import de.fhpotsdam.unfolding.UnfoldingMap;
import de.fhpotsdam.unfolding.geo.Location;
import de.fhpotsdam.unfolding.utils.MapUtils;
import de.fhpotsdam.unfolding.providers.*;
import de.fhpotsdam.unfolding.utils.ScreenPosition;

/**
 * An application with a basic interactive map. You can zoom and pan the map.
 */
public class MapInterface extends PApplet{

    UnfoldingMap map;
    static Comms mIC;
    String instruction;
    
    public synchronized static void setComm(Comms c){
        mIC = c;
    }

    public synchronized void setup() {
            size(1024, 768, P2D);

            map = new UnfoldingMap(this, new Microsoft.HybridProvider());
            map.zoomAndPanTo(new Location(6.316667f, 5.6f), 6);
            MapUtils.createDefaultEventDispatcher(this, map);
    }

    public synchronized void draw() {
        if(mIC.getNew()){
            instruction = mIC.getInstruction();
            
            if(instruction.equals("Left")){
                map.panBy(-10, 0);
            }
            else if(instruction.equals("Right")){
                map.panBy(10, 0);
            }
            else if(instruction.equals("Up")){
                map.panBy(0, -10);
            }
            else if(instruction.equals("Down")){
                map.panBy(0, 10);
            }
            else if(instruction.equals("Zoom In")){
                map.zoomIn();
            }
            else if(instruction.equals("Zoom Out")){
                map.zoomOut();
            }
            mIC.setNew(false);
        }
        
        map.draw();
    }

    public static void startMap(){
        PApplet.main(new String[] { MapInterface.class.getName() });
        //com.bulenkov.game2048.Game2048.main(null);
    }
}
