using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.AI;

public enum ZookeeperState
{
    Patrolling,
    Paused,
    Chasing,
    Searching,
    Stopped
}

public class ZooKeeperAIController : MonoBehaviour {

    public GameObject player; //ref to the player
    public GameObject[] WaypointObjects; //ref to waypoint objects defining patrol path
    public float MaxSearchTime = 10; //time before he gives up searching
    public float waypointPauseTime = 3; //time he waits at each waypoint before moving on
    public float runSpeed = 3.95f; //how fast the nav agent runs
    public float walkSpeed = 1.5f; //how fast nav agent walks
    public float fieldOfView = 30f;
    public float sightDistance = 20f;

    //references to components
    private UnityEngine.AI.NavMeshAgent nav;
    private Animator anim;
    private Rigidbody rb;

    private ZookeeperState currentState = ZookeeperState.Patrolling; //AI state
    private float lostTime; //timer for zookeeper to give up looking for harambe if he gets away
    private float waypointArriveTime; //timer for the zookeeper to pause at each waypoint in his patrol path
    private Vector3 previousPosition; //used to calc movement speed to set animation
    private int nextWaypoint = 0; //index of the next waypoint in the zookeeper's patrol path
    private Vector3[] waypoints; //patrol path waypoints (derived from the WaypointObjects above)
    private TextMesh stateText; //text showing state for debugging/development purposes
    private bool running = false; //whether he's running, used to determine how to scale the animation param
    
    // Use this for initialization
    void Start () {
        waypoints = WaypointObjects.Select(gameObj => gameObj.transform.position).ToArray();
        nav = GetComponent<UnityEngine.AI.NavMeshAgent>();
        rb = GetComponent<Rigidbody>();
        anim = GetComponent<Animator>();
        previousPosition = transform.position;
        stateText = transform.Find("StateText").GetComponent<TextMesh>();
        stateText.text = "Patrolling";
    }
	
	// Update is called once per frame
	void Update () {
        //apply walk/run animation while moving
        //most reliable way to get speed is based on position difference from last frame
        Vector3 curMove = transform.position - previousPosition;
        var curSpeed = curMove.magnitude / Time.deltaTime;
        previousPosition = transform.position;

        //determine how to scale the velocity to set the animation parameter.
        //walking and running are part of a blend tree, where walking is set to 0.5 vSpeed, and run is 1 VSpeed.
        //so, scale running to 1 and scale walking to 0.5
        var scale = running ? runSpeed : walkSpeed * 2;
        anim.SetFloat("VSpeed", curSpeed / scale);

        //check if the zookeeper can see the player
        bool seePlayer = CanSeePlayer();

        //AI state machine
        switch (currentState)
        {
            case ZookeeperState.Patrolling:
                if (seePlayer)
                {
                    ChasePlayer();
                } else
                {
                    //check if we have reached our current waypoint
                    if (arrivedAtWaypoint())
                    {
                        //yep, wait here for pause time
                        waypointArriveTime = Time.time;
                        stateText.text = "Paused";
                        currentState = ZookeeperState.Paused;
                    }
                }
                break;
            case ZookeeperState.Paused:
                if (seePlayer)
                {
                    ChasePlayer();
                } else if (Time.time > waypointArriveTime + waypointPauseTime)
                {
                    ResumePatrol();
                }
                break;
            case ZookeeperState.Chasing:
                if (!seePlayer)
                {
                    //lost sight of the player while chasing him.
                    //keep going toward current waypoint (his last known position)
                    //set up timeout to give up the search
                    lostTime = Time.time;
                    stateText.text = "Searching";
                    currentState = ZookeeperState.Searching;
                } else
                {
                    nav.SetDestination(player.transform.position);
                }
                break;
            case ZookeeperState.Searching:
                if (seePlayer)
                {
                    ChasePlayer();
                }
                if (Time.time > lostTime + MaxSearchTime)
                {
                    ResumePatrol();
                }
                if (arrivedAtWaypoint())
                {
                    running = false;
                    nav.speed = walkSpeed; //walk speed
                    //TODO: turn in place a couple times to look for the player
                }
                break;
            case ZookeeperState.Stopped:
                //game over?
                break;
        }
	}

    private void Stop()
    {
        stateText.text = "Stopped";
        nav.Stop();
        currentState = ZookeeperState.Stopped;
    }

    private void ResumePatrol()
    {
        running = false;
        nav.speed = walkSpeed;
        stateText.text = "Patrolling";
        currentState = ZookeeperState.Patrolling;
        nav.SetDestination(waypoints[nextWaypoint]);
        nextWaypoint++;
        if (nextWaypoint >= waypoints.Length) nextWaypoint = 0;
    }

    private bool arrivedAtWaypoint()
    {
        float dist = nav.remainingDistance;
        return (dist != Mathf.Infinity && nav.pathStatus == NavMeshPathStatus.PathComplete && nav.remainingDistance == 0);
    }

    private void ChasePlayer()
    {
        stateText.text = "Chasing";
        currentState = ZookeeperState.Chasing;
        running = true;
        nav.speed = runSpeed;
        nav.SetDestination(player.transform.position);
    }

    //check if we can see the player
    private bool CanSeePlayer()
    {
        //angle between zookeeper's forward and vector to player
        Vector3 playerPos = player.transform.Find("LookTarget").position;
        Vector3 eyePos = transform.Find("EyePosition").position;
        Vector3 targetDir = playerPos - eyePos;
        float angle = Vector3.Angle(targetDir, transform.forward);
        if (angle < fieldOfView)
        {
            //player is within field of view. next check distance
            float dist = Vector3.Distance(player.transform.position, transform.position);
            if (dist < sightDistance)
            {
                //within viewable distance, next check for obstructions
                //var rayDirection = player.transform.position - transform.position;
                RaycastHit hit;
                if (Physics.Raycast(eyePos, targetDir, out hit))
                {
                    if (hit.transform == player.transform)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void OnCollisionEnter(Collision collision)
    {
        //if we hit the player, it's game over
        if (collision.gameObject == player)
        {
            var controller = player.GetComponent<GorillaUserController>();
            controller.CaughtByZookeeper();
            Stop();
        }
    }
}
