using UnityEngine;
    using System.Collections;
    using System.Collections.Generic;

    [System.Serializable]
    public class AxleInfo {
        public WheelCollider leftWheel;
        public WheelCollider rightWheel;
        public bool motor;
        public bool steering;
    }
     
    public class SimpleCarController : MonoBehaviour {
        public List<AxleInfo> axleInfos; 
        public float maxMotorTorque;
        public float maxSteeringAngle;
	public float brakeTorque = 30000f;
     
        // finds the corresponding visual wheel
        // correctly applies the transform
        public void ApplyLocalPositionToVisuals(WheelCollider collider)
        {
            if (collider.transform.childCount == 0) {
                return;
            }
     
            Transform visualWheel = collider.transform.GetChild(0);
     
            Vector3 position;
            Quaternion rotation;
            collider.GetWorldPose(out position, out rotation);
     
            visualWheel.transform.position = position;
            visualWheel.transform.rotation = rotation;
        }
     
        public void FixedUpdate()
        {
		brakeApply();

            float motor = maxMotorTorque * Input.GetAxis("Vertical");
            float steering = maxSteeringAngle * Input.GetAxis("Horizontal");

		float handBrake = Input.GetKey(KeyCode.X) ? brakeTorque : 0;
     
            foreach (AxleInfo axleInfo in axleInfos) {
                if (axleInfo.steering) {
                    axleInfo.leftWheel.steerAngle = steering;
                    axleInfo.rightWheel.steerAngle = steering;
                }
                if (axleInfo.motor) {
                    axleInfo.leftWheel.motorTorque = -motor;
                    axleInfo.rightWheel.motorTorque = -motor;
                }
                ApplyLocalPositionToVisuals(axleInfo.leftWheel);
                ApplyLocalPositionToVisuals(axleInfo.rightWheel);
            }
        }

	void brakeApply()
	{


		if(Input.GetAxis("Brake") > 0)
		{
			foreach (AxleInfo axleInfo in axleInfos) {

					axleInfo.leftWheel.brakeTorque = Mathf.Infinity;
					axleInfo.rightWheel.brakeTorque = Mathf.Infinity;

			}
		}
		else 
		{
				foreach (AxleInfo axleInfo in axleInfos) {

						axleInfo.leftWheel.brakeTorque = 0;
						axleInfo.rightWheel.brakeTorque = 0;

			}
		
	}
    }
}