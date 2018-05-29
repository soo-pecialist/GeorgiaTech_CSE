using UnityEngine;
using UnityEngine.AI;
using System.Collections;

// Require these components when using this script
[RequireComponent(typeof(Animator))]
[RequireComponent(typeof(CapsuleCollider))]
[RequireComponent(typeof(Rigidbody))]
public class Robot : MonoBehaviour {

    public enum Bot_Area
    {
        INGAME,
        SIDELINE,
        PRISON
    }

    public float animSpeed = 1.5f;              // a public setting for overall animator animation speed


    protected Animator anim;                          // a reference to the animator on the character
    protected AnimatorStateInfo currentBaseState;         // a reference to the current state of the animator, used for base layer
    protected AnimatorStateInfo layer2CurrentState;   // a reference to the current state of the animator, used for layer 2
    protected CapsuleCollider col;                    // a reference to the capsule collider of the character

    protected GameAINavSteeringController AINav;

    public GameObject obHead;
    public SkinnedMeshRenderer obSkin;

    public int bot_idx = 0;
    public int ball_idx = -1;
    public bool is_left_team = false;


    public Bot_Area bot_area;
    public bool stuck_prison = false;
    public bool is_revive = false;

    protected virtual void Start()
    {
        // initialising reference variables
        anim = GetComponent<Animator>();
        col = GetComponent<CapsuleCollider>();

        AINav = GetComponent<GameAINavSteeringController>();
        AINav.Init();
        AINav.waypointLoop = false;
        AINav.stopAtNextWaypoint = false;
        AINav.clearWaypoints();

        if (anim.layerCount == 2)
            anim.SetLayerWeight(1, 1);

        bot_area = Bot_Area.INGAME;
    }


    protected virtual void FixedUpdate()
    {
        if (AINav.waypointsComplete())
        {
            CallBack_NavEnd();
        }
    }

    public void Set_Team(bool is_left)
    {
        is_left_team = is_left;

        Color team_color = Color.white * 0.5f;
        if (is_left_team)
            team_color.r = 1;
        else
            team_color.g = 1;

        team_color.a = 1;
        obSkin.material.color = team_color;
    }

    public virtual void CallBack_NavEnd()
    {
        if (bot_area == Bot_Area.SIDELINE)
        {
            if (stuck_prison) bot_area = Bot_Area.PRISON;
            else if (is_revive) bot_area = Bot_Area.INGAME;
            is_revive = false;
        }
    }

    #region Function_Ball
    public virtual void Obtain_Ball(int b_idx)
    {
        if (bot_area != Bot_Area.INGAME) return;

        ball_idx = b_idx;
        obHead.gameObject.transform.localScale = Vector3.one * 6;
    }

    public void Drop_Ball(bool is_out)
    {
        if (is_out && ball_idx != -1)
            GameManager.instance.Ball_InActive(ball_idx);
        ball_idx = -1;
        obHead.gameObject.transform.localScale = Vector3.one * 3;
    }

    public bool has_ball { get { return ball_idx >= 0; }}

    public virtual void Shot_Ball()
    {
        int temp_ball = ball_idx;
        if (temp_ball < 0)
        {
            Debug.Log("Dont have a Ball");
            return;
        }
        
        Drop_Ball(false);
        GameManager.instance.Ball_Shot(temp_ball, this.transform.localPosition, this.transform.localEulerAngles);
        
    }

    #endregion


    #region OUT and REVIVE
    public virtual void Get_Out(Transform[] Navs)
    {
        // behavior when out
        stuck_prison = true;
        
        // area setting change to side
        bot_area = Bot_Area.SIDELINE;

        // move
        AINav.setWayPoints(Navs);
        AINav.useNavMeshPathPlanning = true;
        AINav.waypointLoop = false;

        // drop the ball
        Drop_Ball(true);

        // update team status situation
        GameManager.instance.Update_TeamState();
    }

    public virtual void Revive(Transform[] Navs)
    {
        // when revived
        stuck_prison = false;
        is_revive = true;

        // area setting change to side
        bot_area = Bot_Area.SIDELINE;
        // move

        AINav.setWayPoints(Navs);
        AINav.useNavMeshPathPlanning = true;
        AINav.waypointLoop = false;
    }
    #endregion
}
