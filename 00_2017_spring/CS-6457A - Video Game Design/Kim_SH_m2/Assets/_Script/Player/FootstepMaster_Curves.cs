//Marshall Heffernan aka itsmars 2015
//Special thanks to Duck, TheKusabi, alucardj, dorpeleg, TonyLi & superpig on the Unity Forums!

//This is the CURVES version of my Footstep script. For this to work, you'll first need to...

//1) Add two floats to your Animator Controller. "FootstepLeft" and "FootstepRight".
//2) Add a CURVE to each footstep animation. See video tutorial for a better explaination.
//3) Tag your Terrain GameObject with "Terrain".
//4) Tag your Non-Terrain GameObject floors with their respective names (for example, a wooden floor would need the tag "Surface_Wood").

//Watch the video tutorial! https://www.youtube.com/watch?v=ISoBKFxQLic

using UnityEngine;
using System.Collections;

[RequireComponent (typeof (AudioSource))]
	//This will add an Audio Source component if you don't have one.

public class FootstepMaster_Curves : MonoBehaviour {
	private Rigidbody rigidBody;
	private GameObject terrainFinder;	//Stores the Terrain GameObject out of the Scene for use later.
	private Terrain terrain;			//Your Terrain (if one's in your scene)
	private TerrainData terrainData;	//Lets us get to the Terrain's splatmap.
	private Vector3 terrainPos;			//Where are we on the Splatmap?
	public static int surfaceIndex = 0;	//The order in which your textures were added to your Terrain.
	private string whatTexture;  		//Holds the FILENAMEs of the Textures in your Terrain.
//	private CharacterController controller;


	private AudioSource mySound;		//The AudioSource component
	private Animator anim;				//Lets us pull floats out of our Animator's curves.
	public static GameObject floor;		//what are we standing on?
	private string currentFoot;			//Each foot does it's own Raycast

	private float currentFrameFootstepLeft;
	private float currentFrameFootstepRight;
	private float lastFrameFootstepLeft;
	private float lastFrameFootstepRight;
//	private float angle;
//	private CapsuleCollider col;

	GameObject[] killEmAll;
	private int i;
	private int j;
	//-----------------------------------------------------------------------------------------

	[Space(5.0f)]
	private float currentVolume;
	[Range(0.0f,1.0f)]
	public float volume = 1.0f;				//Volume slider bar; set this between 0 and 1 in the Inspector.
	[Range(0.0f,0.2f)]
	public float volumeVariance = 0.04f;	//Variance in volume levels per footstep; set this between 0.0 and 0.2 in the inspector. Default is 0.04f.
	private float pitch;
	[Range(0.0f,0.2f)]
	public float pitchVariance = 0.08f;		//Variance in pitch levels per footstep; set this between 0.0 and 0.2 in the inspector. Default is 0.08f.
	[Space(5.0f)]
	public GameObject leftFoot;			//Drag your player's RIG/MESH/BIP/BONE for the left foot here, in the inspector.
	public GameObject rightFoot;		//Drag your player's RIG/MESH/BIP/BONE for the right foot here, in the inspector.
	[Space(5.0f)]
	public AudioClip[] defaults = new AudioClip[0];
	public AudioClip[] dirt = new AudioClip[0];
	public AudioClip[] wood = new AudioClip[0];
	public AudioClip[] metal = new AudioClip[0];
	public AudioClip[] concrete = new AudioClip[0];
	public AudioClip[] sand = new AudioClip[0];

//	public AudioClip[] grass = new AudioClip[0];
//	public AudioClip[] leaves = new AudioClip[0];
//	public AudioClip[] snow = new AudioClip[0];
//	public AudioClip[] water = new AudioClip[0];
	
	[Space(5.0f)]
	[Tooltip("Choose ONE")]
	public bool instantiatedFX;		//Check this checkbox in the inspector if you want your FX to be instantiated.
//	[Tooltip("Choose ONE")]
//	public bool toggledFX;			//Check this checkbox in the inspector if you want your FX to be enabled.

	[Space(5.0f)]
	public GameObject dirtFX;
	public GameObject sandFX;
//	public GameObject snowFX;
//	public GameObject waterFX;
	private Quaternion dirtRotation;
	private Quaternion sandRotation;
//	private Quaternion snowRotation;
//	private Quaternion waterRotation;
	private Vector3 dirtPos;
	private Vector3 sandPos;
//	private Vector3 snowPos;
//	private Vector3 waterPos;
//	private Vector3 characterControllerCenter;

	//-----------------------------------------------------------------------------------------

	//Start
	void Start () {
		anim = GetComponent<Animator>();
		mySound = gameObject.GetComponent<AudioSource>();
		terrainFinder = GameObject.FindGameObjectWithTag ("Terrain");
		rigidBody = GetComponent<Rigidbody> ();
//		controller = GetComponent<CharacterController>();
//		col = GetComponent<CapsuleCollider> ();
//		characterControllerCenter = controller.center;


		if (terrainFinder != null) {	//IS THERE A TERRAIN IN THE SCENE?
			terrain = Terrain.activeTerrain;
			terrainData = terrain.terrainData;
			terrainPos = terrain.transform.position;
		}
	}

	//-----------------------------------------------------------------------------------------

	//Update
	void Update () {

		killEmAll = GameObject.FindGameObjectsWithTag ("Particle"); // delete particle clones after 3 seconds. 
		for (i = 0; i < killEmAll.Length; i++) {
			Destroy (killEmAll [i].gameObject, 3f);
		}

		if (dirtFX != null) {
			dirtFX.transform.rotation = dirtRotation;	//lock the dirt FX particle system's rotation so it doesn't spin (only applies to TOGGLE mode).
			dirtFX.transform.position = dirtPos;		//lock the dirt FX particle system's position so it doesn't follow you as you move (only applies to TOGGLE mode).
		}
		if (sandFX != null) {
			sandFX.transform.rotation = dirtRotation;	//lock the dirt FX particle system's rotation so it doesn't spin (only applies to TOGGLE mode).
			sandFX.transform.position = dirtPos;		//lock the dirt FX particle system's position so it doesn't follow you as you move (only applies to TOGGLE mode).
		}

//		if (snowFX != null) {
//			snowFX.transform.rotation = snowRotation;
//			snowFX.transform.position = snowPos;
//		} 
//		if (waterFX != null) {
//			waterFX.transform.rotation = waterRotation;
//			waterFX.transform.position = waterPos;
//		}
//		Vector3 downward = transform.TransformDirection (-Vector3.up) * 20;
//		Debug.DrawRay (leftFoot.transform.position, downward, Color.green);

		//-----------------------------------------------------------------------------------------
//		RaycastHit Slopehit;
//		if (Physics.Raycast (transform.position, (-Vector3.up), out Slopehit, 1f)) {
////			Vector3 relativePos = transform.position - tran
//			Quaternion q = Quaternion.FromToRotation (Vector3.up, Slopehit.normal);
//			Quaternion l = Quaternion.LookRotation (Slopehit.normal);
////			transform.rotation = q * transform.rotation;
//			transform.rotation= l;
////			print(q);
//		}

	
//			Vector3 targetNormal = Slopehit.normal;
//			Quaternion fromRotation = transform.rotation;
//			Quaternion toRotation = Quaternion.FromToRotation (transform.up,Slopehit.normal) * transform.rotation;


//			angle = Vector3.Angle (Slopehit.normal, Vector3.up);	
//			print (angle);

//			floor = (Slopehit.transform.gameObject);

//			if (floor.tag == "Slope") {
//				rigidBody.AddForce (Physics.gravity * 30f, ForceMode.Acceleration);
//				transform.position = new Vector3 (transform.position.x, curPosY - 0.5f, transform.position.z);
//			}


	//-----------------------------------------------------------------------------------------



		//Check THIS FRAME to see if we need to play a sound for the left foot, RIGHT NOW...
		currentFrameFootstepLeft = anim.GetFloat ("FootstepLeft");		//get left foot's CURVE FLOAT from the Animator Controller, from the LAST FRAME.
//		print ("currentFS: " + currentFrameFootstepLeft + " // " + "lastFS: " + lastFrameFootstepLeft);
		if (currentFrameFootstepLeft > 0 && lastFrameFootstepLeft < 0) {		//is this frame's curve BIGGER than the last frames?
				
			RaycastHit surfaceHitLeft;
			Ray aboveLeftFoot = new Ray (leftFoot.transform.position, -Vector3.up);
				LayerMask layerMask = ~(1 << 18) | (1 << 19); 	//Here we ignore layer 18 and 19 (Player and NPCs). We want the raycast to hit the ground, not people.
				if (Physics.Raycast (aboveLeftFoot, out surfaceHitLeft, 2f, layerMask)) {
//						if (Vector3.Dot (Vector3.up, surfaceHitLeft.normal) < 0.9f) {
////						rigidBody.AddForce (Physics.gravity * 2f, ForceMode.Acceleration);
//					controller.center = new Vector3(0, 1.05f ,0);
//						}
					floor = (surfaceHitLeft.transform.gameObject);
					currentFoot = "Left";				//This will help us place the Instantiated or Toggled FX at the correct position.
					if (floor != null) {
					Invoke ("CheckTexture", 0);		//Play RIGHT FOOTSTEP
//					currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
//					pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
//					mySound.pitch = pitch;
//						mySound.PlayOneShot (defaults [Random.Range (0, 3)], currentVolume);
					}
				if (floor.tag == "Terrain") {
					if (terrainFinder != null) {	//IS THERE A TERRAIN IN THE SCENE?

						surfaceIndex = GetMainTexture (transform.position);
						//Not that it matters, but here we determine what position the Terrain Textures are in.
						//For example, If you added a grass texture, then a dirt, then a rock, you'd have grass=0, dirt=1, rock=2.
						whatTexture = terrainData.splatPrototypes [surfaceIndex].texture.name;
						//Instead of messing around with numbers, we'll just check the texture's filename.
					}
					}
				}
			}
		lastFrameFootstepLeft = anim.GetFloat ("FootstepLeft");	//get left foot's CURVE FLOAT from the Animator Controller, from the CURRENT FRAME.

	//-----------------------------------------------------------------------------------------

			//Check THIS FRAME to see if we need to play a sound for the right foot, RIGHT NOW...
		currentFrameFootstepRight = anim.GetFloat ("FootstepRight");	//get right foot's CURVE FLOAT from the Animator Controller, from the LAST FRAME.
			if (currentFrameFootstepRight < 0 && lastFrameFootstepRight > 0) {		//is this frame's curve SMALLER than last frames?
			
				RaycastHit surfaceHitRight;
				Ray aboveRightFoot = new Ray (rightFoot.transform.position, -Vector3.up);
				LayerMask layerMask = ~(1 << 18) | (1 << 19); 	//Here we ignore layer 18 and 19 (Player and NPCs). We want the raycast to hit the ground, not people.
				if (Physics.Raycast (aboveRightFoot, out surfaceHitRight, 2f, layerMask)) {
//						if (Vector3.Dot (Vector3.up, surfaceHitRight.normal) < 0.9f) {
//				//						rigidBody.AddForce (Physics.gravity * 2f, ForceMode.Acceleration);
//							controller.center = new Vector3(0, 1.05f ,0);
//						}
					floor = (surfaceHitRight.transform.gameObject);
					currentFoot = "Right";				//This will help us place the Instantiated or Toggled FX at the correct position.
//				print (floor);
					if (floor != null) {
						Invoke ("CheckTexture", 0);		//Play RIGHT FOOTSTEP
//						currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
//						pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
//						mySound.pitch = pitch;
//							mySound.PlayOneShot (defaults [Random.Range (0, 3)], currentVolume);
				}
				if (floor.tag == "Terrain") {
					if (terrainFinder != null) {	//IS THERE A TERRAIN IN THE SCENE?

						surfaceIndex = GetMainTexture (transform.position);
						//Not that it matters, but here we determine what position the Terrain Textures are in.
						//For example, If you added a grass texture, then a dirt, then a rock, you'd have grass=0, dirt=1, rock=2.
						whatTexture = terrainData.splatPrototypes [surfaceIndex].texture.name;
						//Instead of messing around with numbers, we'll just check the texture's filename.
					}
				}
				}
			}																																		//???????????????????????????????????????????????????????
		lastFrameFootstepRight = anim.GetFloat ("FootstepRight");	//get right foot's CURVE FLOAT from the Animator Controller, from the CURRENT FRAME.
	} //END OF UPDATE
	//----------------------------------------------------------------------------------------
	
	//Puts ALL TEXTURES from the Terrain into an array, represented by floats (0=first texture, 1=second texture, etc).
	private float[] GetTextureMix(Vector3 WorldPos){
		if (terrainFinder != null) {	//IS THERE A TERRAIN IN THE SCENE?
			// calculate which splat map cell the worldPos falls within
			int mapX = (int)(((WorldPos.x - terrainPos.x) / terrainData.size.x) * terrainData.alphamapWidth);
			int mapZ = (int)(((WorldPos.z - terrainPos.z) / terrainData.size.z) * terrainData.alphamapHeight);
			// get the splat data for this cell as a 1x1xN 3d array (where N = number of textures)
			float[,,] splatmapData = terrainData.GetAlphamaps (mapX, mapZ, 1, 1);
			float[] cellMix = new float[ splatmapData.GetUpperBound (2) + 1 ]; //turn splatmap data into float array
			for (int n=0; n<cellMix.Length; n++) {
				cellMix [n] = splatmapData [0, 0, n];
			}return cellMix;
		} else return null;	//THERE'S NO TERRAIN IN THE SCENE! DON'T DO THE ABOVE STUFF.
	}

	//Takes the "GetTextureMix" float array from above and returns the MOST DOMINANT texture at Player's position.
	private int GetMainTexture(Vector3 WorldPos){
		if (terrainFinder != null) {	//IS THERE A TERRAIN IN THE SCENE?
			float[] mix = GetTextureMix (WorldPos);
			float maxMix = 0;
			int maxIndex = 0;
			for (int n=0; n<mix.Length; n++){
				if (mix [n] > maxMix){
					maxIndex = n;
					maxMix = mix [n];
				}
			}return maxIndex;
		} else return 0;	//THERE'S NO TERRAIN IN THE SCENE! DON'T DO THE ABOVE STUFF.
	}
	
	//-----------------------------------------------------------------------------------------
	
	void CheckTexture(){
		//First we'll check to see if the player just stepped on a NON-TERRAIN GameObject. TAG YOUR FLOORS APPROPRIATELY (for example, tag a wooden floor with "Surface_Wood").
		//Feel free to make your own sound categories.

//		if (floor.tag == ("Surface_Grass"))
//			Invoke ("PlayGrass", 0);
//		if (floor.tag == ("Surface_Leaves"))
//			Invoke ("PlayLeaves", 0);
		if (floor.tag == ("Surface_Dirt"))
			Invoke ("PlayDirt", 0);	
		if (floor.tag == ("Surface_Sand"))
			Invoke ("PlaySand", 0);
		if (floor.tag == ("Surface_Concrete"))
			Invoke ("PlayConcrete", 0);
		if (floor.tag == ("Surface_Wood"))
			Invoke ("PlayWood", 0);
		if (floor.tag == ("Surface_Metal"))
			Invoke ("PlayMetal", 0);
//		if (floor.tag == ("Surface_Snow"))
//			Invoke ("PlaySnow", 0);
//		if (floor.tag == ("Surface_Water"))
//			Invoke ("PlayWater", 0);
		if (floor.tag == ("Untagged") || floor.tag == ("Ground"))
			Invoke ("PlayDefault", 0);
		if (floor.tag == ("Terrain")) {
			//Now we'll check to see if the player just stepped on a Terrain. TAG YOUR TERRAIN with "Terrain".
			//Feel free to make your own sound categories.
							
			//This is the coolest part; We check the TEXTURE NAME in your Project folder, then "Invoke" the corresponding Function/Method.
			//We'll check for UpperCase, lowercase and ALL CAPS filenames. The "keyword" can appear anywhere in the title!
			//For example, if you had a texture called "medieval_grass_brown" it would "invoke" a sound from the grass array.

//			if (whatTexture.Contains ("grass") == true || whatTexture.Contains ("Grass") == true || whatTexture.Contains ("GRASS") == true)
//				Invoke ("PlayGrass", 0);
//			else if (whatTexture.Contains ("leaves") == true || whatTexture.Contains ("Leaves") == true|| whatTexture.Contains ("LEAVES") == true)
//				Invoke ("PlayLeaves", 0);
			if (whatTexture.Contains ("dirt_texture") == true || whatTexture.Contains ("Dirt") == true|| whatTexture.Contains ("DIRT") == true)
				Invoke ("PlayDirt", 0);
			else if (whatTexture.Contains ("sand_texture") == true || whatTexture.Contains ("Sand") == true|| whatTexture.Contains ("SAND") == true)
				Invoke ("PlaySand", 0);
			else if (whatTexture.Contains ("concrete_texture") == true || whatTexture.Contains ("Concrete") == true|| whatTexture.Contains ("CONCRETE") == true)
				Invoke ("PlayConcrete", 0);
			else if (whatTexture.Contains ("wood") == true || whatTexture.Contains ("Wood") == true|| whatTexture.Contains ("WOOD") == true)
				Invoke ("PlayWood", 0);
			else if (whatTexture.Contains ("metal_texture") == true || whatTexture.Contains ("Metal") == true|| whatTexture.Contains ("METAL") == true)
				Invoke ("PlayMetal", 0);
//			else if (whatTexture.Contains ("snow") == true || whatTexture.Contains ("Snow") == true|| whatTexture.Contains ("SNOW") == true)
//				Invoke ("PlaySnow", 0);
//			else if (whatTexture.Contains ("water") == true || whatTexture.Contains ("Water") == true|| whatTexture.Contains ("WATER") == true)
//				Invoke ("PlayWater", 0);
			else Invoke ("PlayDefault", 0);
				}
			}

	//-----------------------------------------------------------------------------------------

	//INVOKED FUNCTIONS BELOW. Plays a Random sound from an array at a varied pitch and volume.

	//I've included support for three FX Prefabs below (a dust cloud for dirt, white puffs for snow and splashes for water). Feel free to add more.

	//If you're Instantiating your FX, make sure it has a script to delete itself after a while, to keep your Hierarchy clean. Drag the prefab into the inspector FROM YOUR PROJECT WINDOW.
	//If you're Toggling your FX, just make sure it's a short enough particle system so that it won't look wierd when it turns off, abruptly. Drag the FX into the inspector FROM YOUR PLAYER.
	
//	void PlayGrass(){
//		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
//		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
//		mySound.pitch = pitch;
//		if (grass.Length > 0) {
//			mySound.PlayOneShot (grass [Random.Range (0, grass.Length)], currentVolume);
//		} else Debug.LogError ("trying to play grass sound, but no grass sounds in array!");
//	}
//
//	void PlayLeaves(){
//		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
//		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
//		mySound.pitch = pitch;
//		if (leaves.Length > 0) {
//			mySound.PlayOneShot (leaves [Random.Range (0, leaves.Length)], currentVolume);
//		} else Debug.LogError ("trying to play leaves sound, but no leaves sounds in array!");
//	}
	
	void PlayDirt(){		//Supports toggling or instantiating a "dirtFX" AT THE PLAYER'S CURRENT FOOT which could be a dust cloud particle system, or something...
		currentVolume = (volume + UnityEngine.Random.Range (-volumeVariance, volumeVariance));
		pitch = (1.0f + Random.Range (-pitchVariance, pitchVariance));
		mySound.pitch = pitch;
		if (dirt.Length > 0) {
			mySound.PlayOneShot (dirt [Random.Range (0, dirt.Length)], currentVolume);		//play a random sound from it's corresponding array.
		} else Debug.LogError ("trying to play dirt sound, but no dirt sounds in array!");
		if (dirtFX != null) {		//Is a prefab in the GameObject box, in the Inspector?
			if (instantiatedFX == true) {						//Code for INSTANTIATING a Particle System at the player's feet.
				if (currentFoot == ("Left")) {
					Instantiate (dirtFX, (leftFoot.transform.position + new Vector3 (0, 0.05f, 0)), transform.rotation);	//Instantiates the prefab at the left foot + 0.05f (slightly above ground).
				} else if (currentFoot == ("Right")) {
					Instantiate (dirtFX, (rightFoot.transform.position+ new Vector3 (0, 0.05f, 0)), transform.rotation);	//Instantiates the prefab at the right foot + 0.05f (slightly above ground).
				}
			}
//			if (toggledFX == true) {							//Code for ENABLING a Particle System at the player's feet.
//				if (currentFoot == ("Left")) {
//					if (dirtFX != null) {
//						dirtFX.SetActive (false);
//						dirtFX.SetActive (true);
//						dirtFX.transform.position = leftFoot.transform.position;
//						dirtRotation = dirtFX.transform.rotation;
//						dirtPos = dirtFX.transform.position;
//					}
//				} else if (currentFoot == ("Right")) {
//					if (dirtFX != null) {
//						dirtFX.SetActive (false);
//						dirtFX.SetActive (true);
//						dirtFX.transform.position = rightFoot.transform.position;
//						dirtRotation = dirtFX.transform.rotation;
//						dirtPos = dirtFX.transform.position;
//					}
//				}
//			}
		}
	}

	void PlaySand(){
		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
		mySound.pitch = pitch;
		if (sand.Length > 0) {
			mySound.PlayOneShot (sand [Random.Range (0, sand.Length)], currentVolume);
		} else Debug.LogError ("trying to play sand sound, but no sand sounds in array!");
		if (sandFX != null) {		//Is a prefab in the GameObject box, in the Inspector?
			if (instantiatedFX == true) {						//Code for INSTANTIATING a Particle System at the player's feet.
				if (currentFoot == ("Left")) {
					Instantiate (sandFX, (leftFoot.transform.position + new Vector3 (0, 0.05f, 0)), transform.rotation);	//Instantiates the prefab at the left foot + 0.05f (slightly above ground).
				} else if (currentFoot == ("Right")) {
					Instantiate (sandFX, (rightFoot.transform.position+ new Vector3 (0, 0.05f, 0)), transform.rotation);	//Instantiates the prefab at the right foot + 0.05f (slightly above ground).
				}
			}
		}
	}

	void PlayConcrete(){
		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
		mySound.pitch = pitch;
		if (concrete.Length > 0) {
			mySound.PlayOneShot (concrete [Random.Range (0, concrete.Length)], currentVolume);
		} else Debug.LogError ("trying to play concrete sound, but no concrete sounds in array!");
	}
	
	void PlayWood(){
		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
		mySound.pitch = pitch;
		if (wood.Length > 0) {
			mySound.PlayOneShot (wood [Random.Range (0, wood.Length)], 0.9f);
		} else Debug.LogError ("trying to play wood sound, but no wood sounds in array!");
	}

	void PlayMetal(){
		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
		mySound.pitch = pitch;
		if (metal.Length > 0) {
			mySound.PlayOneShot (metal [Random.Range (0, metal.Length)], currentVolume);
		} else Debug.LogError ("trying to play metal sound, but no metal sounds in array!");
	}

	void PlayDefault(){
		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
		mySound.pitch = pitch;
		if (defaults.Length > 0) {
			mySound.PlayOneShot (defaults [Random.Range (0, defaults.Length)], currentVolume);
		} else Debug.LogError ("trying to play the default sound, but no default sounds in array!");
	}

//	void PlaySnow(){		//Supports toggling or instantiating a "snowFX" AT THE PLAYER'S CURRENT FOOT which could be a white poofy particle system, or something...
//		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
//		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
//		mySound.pitch = pitch;
//		if (snow.Length > 0) {
//			mySound.PlayOneShot (snow [Random.Range (0, snow.Length)], currentVolume);
//		} else
//			Debug.LogError ("trying to play snow sound, but no snow sounds in array!");
//		if (snowFX != null) {		//Is a Prefab in the SnowFX GameObject slot, in the Inspector?
//			if (instantiatedFX == true) {					//Code for INSTANTIATING a Particle System at the player's feet.
//				if (currentFoot == ("Left")) {
//					Instantiate (snowFX, (leftFoot.transform.position + new Vector3 (0, 0.05f, 0)), transform.rotation);	//Instantiates the prefab at the left foot + 0.05f (slightly above ground).
//				} else if (currentFoot == ("Right")) {
//					Instantiate (snowFX, (rightFoot.transform.position + new Vector3 (0, 0.05f, 0)), transform.rotation);	//Instantiates the prefab at the right foot + 0.05f (slightly above ground).
//				}
//			}
//			if (toggledFX == true){							//Code for ENABLING a Particle System at the player's feet.
//				if (currentFoot == ("Left")) {
//					if (snowFX != null) {
//						snowFX.SetActive (false);
//						snowFX.SetActive (true);
//						snowFX.transform.position = leftFoot.transform.position;
//						snowRotation = snowFX.transform.rotation;
//						snowPos = snowFX.transform.position;
//					}
//				} else if (currentFoot == ("Right")) {
//					if (snowFX != null) {
//						snowFX.SetActive (false);
//						snowFX.SetActive (true);
//						snowFX.transform.position = rightFoot.transform.position;
//						snowRotation = snowFX.transform.rotation;
//						snowPos = snowFX.transform.position;
//					}
//				}
//			}
//		}
//	}

//	void PlayWater(){		//Supports toggling or instantiating a "waterFX" AT THE SURFACE OF THE WATER, which could be a splashy particle system, or something...
//		currentVolume = (volume + UnityEngine.Random.Range(-volumeVariance, volumeVariance));
//		pitch = (1.0f + Random.Range(-pitchVariance, pitchVariance));
//		mySound.pitch = pitch;
//		if (water.Length > 0) {
//			mySound.PlayOneShot (water [Random.Range (0, water.Length)], currentVolume);
//		} else Debug.LogError ("trying to play water sound, but no water sounds in array!");
//		if (waterFX != null) {		//Water is a bit different than the others: We want the water splash prefab to be placed at the SURFACE of the water, not at the bottom of the feet.
//			if (instantiatedFX == true){						//Code for INSTANTIATING a Particle System at the player's feet.
//				if (currentFoot == ("Left")) {
//					RaycastHit leftWaterHit;
//					Ray aboveWaterLeft = new Ray (leftFoot.transform.position + new Vector3(0,1.5f,0), Vector3.down);
//					LayerMask layerMask = ~(1 << 18) | (1 << 19); 	//Here we ignore layer 18 and 19 (Player and NPCs). We want the raycast to hit the ground, not people.
//					if (Physics.Raycast (aboveWaterLeft, out leftWaterHit, 2f, layerMask)) {
//						Instantiate (waterFX, (leftWaterHit.point + new Vector3 (0, 0.05f, 0)), transform.rotation);
//					}
//				}
//				else if  (currentFoot == ("Right")) {
//					RaycastHit rightWaterHit;
//					Ray aboveWaterRight = new Ray (rightFoot.transform.position + new Vector3(0,1.5f,0), Vector3.down);
//					LayerMask layerMask = ~(1 << 18) | (1 << 19); 	//Here we ignore layer 18 and 19 (Player and NPCs). We want the raycast to hit the ground, not people.
//					if (Physics.Raycast (aboveWaterRight, out rightWaterHit, 2f, layerMask)) {
//						Instantiate (waterFX, (rightWaterHit.point + new Vector3 (0, 0.05f, 0)), transform.rotation);
//					}
//				}
//			}
//			if (toggledFX == true){							//Code for ENABLING a Particle System at the player's feet.
//				if (currentFoot == ("Left")) {
//					RaycastHit leftWaterHit;
//					Ray aboveWaterLeft = new Ray (leftFoot.transform.position + new Vector3(0,1.5f,0), Vector3.down);
//					LayerMask layerMask = ~(1 << 18) | (1 << 19); 	//Here we ignore layer 18 and 19 (Player and NPCs). We want the raycast to hit the ground, not people.
//					if (Physics.Raycast (aboveWaterLeft, out leftWaterHit, 2f, layerMask)) {
//						waterFX.SetActive (false);
//						waterFX.SetActive (true);
//						waterFX.transform.position = leftWaterHit.point;
//						waterRotation = waterFX.transform.rotation;
//						waterPos = waterFX.transform.position;
//					}
//				}
//				else if (currentFoot == ("Right")) {
//					RaycastHit rightWaterHit;
//					Ray aboveWaterRight = new Ray (rightFoot.transform.position + new Vector3(0,1.5f,0), Vector3.down);
//					LayerMask layerMask = ~(1 << 18) | (1 << 19); 	//Here we ignore layer 18 and 19 (Player and NPCs). We want the raycast to hit the ground, not people.
//					if (Physics.Raycast (aboveWaterRight, out rightWaterHit, 2f, layerMask)) {
//						waterFX.SetActive (false);
//						waterFX.SetActive (true);
//						waterFX.transform.position = rightWaterHit.point;
//						waterRotation = waterFX.transform.rotation;
//						waterPos = waterFX.transform.position;
//					}
//				}
//			}
//		}
//	}


	void LandJump()
	{
		AudioClip clip=null;
		float maxvol=1.0f;
		RaycastHit hit;
		if (Physics.Raycast(transform.position, -Vector3.up, out hit, 5.2f))//make sure this cast is long enough to hit the ground
		{
			if (hit.collider.CompareTag("Ground") || hit.collider.CompareTag("Terrain")||hit.collider.CompareTag("Surface_Dirt") || hit.collider.CompareTag("Surface_Sand") || hit.collider.CompareTag("Surface_Concrete") || hit.collider.CompareTag("Surface_Wood") || hit.collider.CompareTag("Surface_Metal"))
			{
				clip=defaults[4];
				maxvol=UnityEngine.Random.Range(0.2f, 0.5f);
			}
		}
		if (clip != null)
			mySound.PlayOneShot(clip, maxvol);
	}

	public void LandingSound(){
		AudioClip clip = defaults[5];

		mySound.PlayOneShot (clip, 0.8f);
	}

}