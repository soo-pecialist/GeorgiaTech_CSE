using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class EventManager : MonoBehaviour {

	private Dictionary <string, UnityEvent> eventDictionary;

	private static EventManager eventManager; 

	private static EventManager instance
	{
		get{ 
			if (!eventManager) {
				eventManager = FindObjectOfType (typeof(EventManager)) as EventManager;

				if (!eventManager) {
					Debug.LogError ("There needs to be one active EventManager script on a GameObject in your scene.");
				} else {
					eventManager.Init ();
				}
			}
			return eventManager;	
		}
	}

	void Init (){
		if (eventDictionary == null) {
			eventDictionary = new Dictionary<string, UnityEvent> ();
		}
	}

	public static void StartListening(string eventName, UnityAction listener){ // register listeners
		UnityEvent thisEvent = null;
		if (instance.eventDictionary.TryGetValue (eventName, out thisEvent)) { // if we have evenName 
			thisEvent.AddListener (listener); // add "listener" to thisEvent
		} else { // if we don't have eventName
			thisEvent = new UnityEvent ();	
			thisEvent.AddListener (listener); // add listener to thisEvent
			instance.eventDictionary.Add (eventName, thisEvent);
		}
	}

	public static void StopListening (string eventName, UnityAction listener){ // unregister listeners
		if (eventManager == null) {	return;	}

		UnityEvent thisEvent = null;
		if (instance.eventDictionary.TryGetValue (eventName, out thisEvent)) {
			thisEvent.RemoveListener (listener);
		}
	}

	public static void TriggerEvent	(string eventName){ // this is how we gonna trigger event
		UnityEvent thisEvent = null;
		if (instance.eventDictionary.TryGetValue (eventName, out thisEvent)) {
			thisEvent.Invoke (); // call for thisEvent
		}
	}
}
