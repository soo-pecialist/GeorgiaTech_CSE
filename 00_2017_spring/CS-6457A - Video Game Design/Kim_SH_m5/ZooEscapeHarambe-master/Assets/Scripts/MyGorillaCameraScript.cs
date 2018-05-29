using UnityEngine;
using System.Collections;

public class MyGorillaCameraScript : MonoBehaviour {
	public GameObject target;
	public float turnSpeed=.2f;
	public GameObject gorillaCamera;
	float cameraAngleX=195f;
	public float cameraAngleY=90f;
	public float cameraDistance=3f;
    public float cameraHeight = -2f;


    public void Start(){
		Quaternion arotation = Quaternion.identity;
		Vector3 eua = Vector3.zero;
		eua.y = 360f-cameraAngleY;
		eua.z = 0f;
		eua.x = 180f+cameraAngleX;
		arotation.eulerAngles = eua;
		transform.localRotation= arotation;
	}
	
	void Update(){
		if (Input.GetKey (KeyCode.Mouse1)) {
			cameraAngleY+= Input.GetAxis("Mouse X");
			cameraAngleX+= Input.GetAxis("Mouse Y");
		}
		CameraRotationX ();
		CameraRotationY ();
		cameraDistance=cameraDistance+Time.deltaTime*Input.GetAxis ("Mouse ScrollWheel");
		gorillaCamera.transform.localPosition = new Vector3 (0f,cameraDistance,cameraHeight*cameraDistance);
	}
	
	public void SetTarget(GameObject aTarget){
		aTarget = target;
	}
	
	public void CameraRotationX(){
		Quaternion arotation = Quaternion.identity;
		Vector3 eua = Vector3.zero;
		eua.y = 360f-cameraAngleY;
		eua.z = 0f;
		eua.x = 180f+cameraAngleX;
		arotation.eulerAngles = eua;
		transform.localRotation= arotation;
	}
	public void CameraRotationY(){
		Quaternion arotation = Quaternion.identity;
		Vector3 eua = Vector3.zero;
		eua.y = 360f-cameraAngleY;
		eua.z = 0f;
		eua.x = 180f+cameraAngleX;
		arotation.eulerAngles = eua;
		transform.localRotation= arotation;
	}
	void FixedUpdate(){
		transform.position = Vector3.Lerp (transform.position,target.transform.position,Time.deltaTime*5f);
	}
}
