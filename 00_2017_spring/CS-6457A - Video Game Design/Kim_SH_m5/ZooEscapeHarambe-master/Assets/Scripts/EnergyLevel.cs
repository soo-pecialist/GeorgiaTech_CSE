using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class EnergyLevel : MonoBehaviour {
	
	public Image visualhealth;
	public float currenthealth=0;
	public float maxhealth=200;
	public float rzlt = 0.0f;

	void Start () {
		currenthealth = maxhealth;


	}
	
	// Update is called once per frame
	void Update () 
	{
		HealthManager ();
		currenthealth= currenthealth-0.05f;
	}

	void OnTriggerEnter(Collider other)
	{
		if (other.tag == "Traffic" && currenthealth > 0) 
		{
			
			currenthealth=currenthealth-30f;
		}
		else if(other.tag=="Health")
		{
			currenthealth =currenthealth+30f;
		}

	}

	void HealthManager ()
	{
		
		rzlt=(currenthealth/maxhealth);
		visualhealth.fillAmount= (rzlt); 
		if(visualhealth.fillAmount==0){
			SceneManager.LoadScene (10);
			Time.timeScale = 1f;
		}

	}
}
