using UnityEngine;
using System.Collections;

public class wheelColliderMover : MonoBehaviour {
	Rigidbody rbody;
	public Vector3 COM = Vector3.zero;
	public WheelCollider[] wc;
	public Transform[] tyresMesh = new Transform[4];
	public int wc_Length;

	public float motorForce;
	public float steerForce;


	// Use this for initialization
	void Start () {
		rbody = GetComponent<Rigidbody> ();
		rbody.centerOfMass = COM;
	}
	
	// Update is called once per frame
	void Update () 
	{

		brakeApply();
		for(int i = 0; i < wc_Length; i++)
		{
			wc [i].motorTorque = Input.GetAxis ("Vertical") * -motorForce;
			wc[i].transform.Rotate(Vector3.forward*Time.deltaTime*motorForce*Input.GetAxis("Vertical"));

        }
		wc [0].steerAngle = Input.GetAxis ("Horizontal") * steerForce;
		wc [1].steerAngle = Input.GetAxis ("Horizontal") * steerForce;
        
		//UpdateTyresMesh ();
	}


	void brakeApply()
	{


		if(Input.GetAxis("Brake") > 0)
		{
		for(int i = 0; i < wc_Length; i++)
		{
				wc [i].brakeTorque = Mathf.Infinity;

		}
		}
		else 
		{
			for(int i = 0; i < wc_Length; i++)
			{
				wc [i].brakeTorque = 0;

			}
		}
	}
	void UpdateTyresMesh()
	{
		
		Quaternion quat;
		Vector3 pos;

		for(int i=0; i <4; i++)
		{
			wc[i].GetWorldPose(out pos, out quat);

			tyresMesh [i].position = pos;
			tyresMesh [i].rotation = quat;
		}
	}
}
