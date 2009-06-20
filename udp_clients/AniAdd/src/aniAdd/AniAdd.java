/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd;

import aniAdd.Communication.ComEvent;
import aniAdd.Communication.ComListener;
import aniAdd.misc.Mod_Memory;
import gui.Mod_GUI;
import java.util.ArrayList;
import java.util.Collection;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import processing.Mod_EpProcessing;
import udpApi.Mod_UdpApi;

/**
 *
 * @author Arokh
 */
public class AniAdd implements IAniAdd{
    TreeMap<String, Module> modules;
    EventHandler eventHandler;
    Mod_Memory mem;

    public AniAdd() {
        modules = new TreeMap<String, Module>();
        eventHandler = new EventHandler();

        Module mod;
        mod = new Mod_Memory();
        modules.put(mod.ModuleName(), mod);
        eventHandler.AddEventHandler(mod);
        mem = (Mod_Memory)mod;
          
        mod = new Mod_EpProcessing();
        modules.put(mod.ModuleName(), mod);
        eventHandler.AddEventHandler(mod);

        mod = new Mod_UdpApi();
        modules.put(mod.ModuleName(), mod);
        eventHandler.AddEventHandler(mod);

        mod = new Mod_GUI();
        modules.put(mod.ModuleName(), mod);
        eventHandler.AddEventHandler(mod);
    }

    public void Start(){
        ComFire(new ComEvent(this, ComEvent.eType.Information, Module.eModState.Initializing));
        mem.put("FirstStart", 2);

        for (Module module : modules.values()) {
            System.out.println("Initializing: " + module.ModuleName());
            module.Initialize(this);
        }

        boolean allModsInitialized = false;
        while(!allModsInitialized){
            try {Thread.sleep(100);} catch (InterruptedException ex) {}

            allModsInitialized = true;
            for (Module module : modules.values()) { allModsInitialized &= module.ModState() == Module.eModState.Initialized; }
        }

        ComFire(new ComEvent(this, ComEvent.eType.Information, Module.eModState.Initialized));
    }
    
    public void Stop(){
        ComFire(new ComEvent(this, ComEvent.eType.Information, Module.eModState.Terminating));

        for (Module module : modules.values()) {
            System.out.println("Terminating: " + module.ModuleName());
            module.Terminate();
        }

        boolean allModsTerminated = false;
        while(!allModsTerminated){
            try {Thread.sleep(100);} catch (InterruptedException ex) {}
            
            allModsTerminated = true;
            for (Module module : modules.values()) { allModsTerminated &= module.ModState() == Module.eModState.Terminated; }
        }

        
        ComFire(new ComEvent(this, ComEvent.eType.Information, Module.eModState.Terminated));
    }

    public Module GetModule(String modName) { return modules.get(modName); }

    public Collection<Module> GetModules(){return modules.values();}

    class EventHandler implements ComListener{
        public void AddEventHandler(Module mod){ mod.AddComListener(this);}
        public void EventHandler(ComEvent comEvent) {
            System.out.print("Event: " + ((Module)comEvent.getSource()).ModuleName() + " ");
            System.out.print(comEvent.Type());
            for (int i=0; i<comEvent.ParamCount(); i++) {
                System.out.print(" " + comEvent.Params(i));
            }
            System.out.println("");
        }
    }


    //Com System
	private ArrayList<ComListener> listeners = new ArrayList<ComListener>();
    protected void ComFire(ComEvent comEvent){
        System.out.print("AniAdd Event: ");
        for (int i=0; i<comEvent.ParamCount(); i++) {
            System.out.print(" " + comEvent.Params(i));
        }
        System.out.println("");

        for (ComListener listener : listeners) {
            listener.EventHandler(comEvent);
        }
    }
	public void AddComListener(ComListener comListener){ listeners.add(comListener); }
	public void RemoveComListener(ComListener comListener){ listeners.remove(comListener); }
    //Com System End
}
