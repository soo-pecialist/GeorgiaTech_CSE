
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;


public class MyGameController : MonoBehaviour {

    public GameObject mycam;
	public Text GoText;
	public GameObject arrow5;
	public GameObject arrow6;
    public GameObject failCanvas;
	//public Camera carcam;
	
	void Start () {
		
        
	}




	void OnCollisionEnter(Collision other){
		
		if (other.gameObject.name == "GorillaModelLowPoly") {
			arrow5.SetActive (true);

			other.gameObject.SetActive (false);
			this.gameObject.GetComponent<SimpleCarController> ().enabled= true;
            mycam.GetComponent<MyGorillaCameraScript>().target=this.gameObject;
            mycam.GetComponent<MyGorillaCameraScript>().cameraHeight = -6f;
			mycam.transform.parent = this.transform ;
			mycam.GetComponent<MyGorillaCameraScript> ().cameraAngleY = 180f;
		
           
		}
        else if(other.gameObject.tag=="obstacle")
        {
            if (failCanvas != null)
            {
                failCanvas.SetActive(true);
            }
            else {
                GoText.text = "Game Over!";
            }

			Time.timeScale = 0.0f;
            Debug.Log("GameOver");
		}

	}


	void OnTriggerEnter (Collider other)
	{
		if (other.gameObject.tag == "Arrow5") {
			Destroy (arrow5.gameObject);
			arrow6.SetActive (true);
		}if (other.gameObject.tag == "Arrow6") {
			Destroy (arrow6.gameObject);
		}
	}
}
