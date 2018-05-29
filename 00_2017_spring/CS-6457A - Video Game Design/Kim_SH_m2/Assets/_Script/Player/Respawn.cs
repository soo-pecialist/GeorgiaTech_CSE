using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Respawn : MonoBehaviour {

	public GameObject ragdoll;
	public Transform playerSpawnPoints; // The parent of the spawn points

//	private bool reSpawn = false;
	private Transform[] spawnPoints;


	// Use this for initialization
	void Start () {
		spawnPoints = playerSpawnPoints.GetComponentsInChildren<Transform> ();
	}

//	// Update is called once per frame
//	void Update () {
//		if (reSpawn) {
//			RespawnPlayer ();
//			reSpawn = false;
//		} 
//	}

	public void RespawnPlayer (){
		int i = Random.Range (1, spawnPoints.Length);
//		Instantiate (ragdoll, spawnPoints [i].transform.position, Quaternion.identity);
		transform.position = spawnPoints [i].transform.position;
		transform.rotation = Quaternion.Euler (0, 0, 0);
	}
//TODO respawning sound		
}
