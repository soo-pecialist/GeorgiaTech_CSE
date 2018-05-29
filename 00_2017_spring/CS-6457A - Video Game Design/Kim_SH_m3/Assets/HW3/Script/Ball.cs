using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ball : MonoBehaviour {

    public enum Ball_State
    {
        INACTIVE,
        WAIT,
        POSSESED,
        SHOT,
        HIT
    }

    public int b_idx;
    public int res_idx;
    public bool is_leftteam = false;

    public Ball_State b_state = Ball_State.INACTIVE;
    public Ball_State B_State { get { return b_state; } set { b_state = value; } }

    private void Start()
    {
        for (int i = 0; i < GameManager.instance.Borders.Length; ++i)
            Physics.IgnoreCollision(GameManager.instance.Borders[i], GetComponent<Collider>());
        for (int i = 0; i < GameManager.instance.Ball_List.Count; ++i)
            Physics.IgnoreCollision(GameManager.instance.Ball_List[i].GetComponent<Collider>(), GetComponent<Collider>());
    }

    // Update is called once per frame
    void Update () {
        
        switch(b_state)
        {
            case Ball_State.INACTIVE:
                break;
            case Ball_State.WAIT:
                break;
            case Ball_State.POSSESED:
                break;
            case Ball_State.SHOT:
                if (is_out)
                {
                    GameManager.instance.Ball_InActive(b_idx);
                    Debug.Log("OUT");
                }
                break;
            case Ball_State.HIT:
                Debug.Log("Ball HIT SOMETHING");
                GameManager.instance.Ball_InActive(b_idx);
                
                break;
        }
	}



    private bool is_out { get { return this.gameObject.transform.localPosition.y < 0; }}

    public bool is_busy { get
        {
            return b_state != Ball_State.INACTIVE;
        }
    }

    

    private void OnCollisionEnter(Collision collision)
    {
        //// Ground, Obstacles, Player, Ball
        bool is_ground = collision.gameObject.CompareTag("ground");
        bool is_player = collision.gameObject.CompareTag("Player");
        
        switch (b_state)
        {
            case Ball_State.WAIT:
                if (is_player)
                {
                    Robot got_bot = null;
                    got_bot = collision.gameObject.GetComponent<Robot>();
                    if (got_bot == null)
                    {
                        Debug.LogError("BALL COLLISION ENTER <WAIT> : ROBOT ERROR!!!!!!!!!!!!!!!!!!!");
                        return;
                    }

                    if(got_bot.has_ball)
                    {
                        Debug.Log("Already has a Ball");
                        GameManager.instance.Ball_InActive(b_idx);
                        return;
                    }
                    Debug.Log("Ball is possesed");
                    GameManager.instance.Ball_Store(b_idx, got_bot.bot_idx);
                    b_state = Ball_State.POSSESED;
                }
                break;
            case Ball_State.SHOT:
                if (is_player)
                {
                    Debug.Log("Ball Hit the Player");
                    /// if player is opponent
                    Robot hit_bot = null;
                    hit_bot = collision.gameObject.GetComponent<Robot>();
                    if (hit_bot == null)
                    {
                        Debug.LogError("BALL COLLISION ENTER <FLT> : BOT ERROR!!!!!!!!!!!!!!!!!!!");
                        b_state = Ball_State.HIT;
                        return;
                    }

                    if (hit_bot.is_left_team == is_leftteam) // by teammate
                    {
                        if (hit_bot.bot_area == Robot.Bot_Area.PRISON) // SAVE PRISONER
                        {
                            Debug.Log("SAVE THE PRISONER!!!");
                            GameManager.instance.Robot_Revive(hit_bot.bot_idx);
                        }
                    }
                    else // by enemy
                    {
                        if (hit_bot.bot_area == Robot.Bot_Area.INGAME) // OUT
                        {
                            Debug.Log("GET OUT!!!!");
                            GameManager.instance.Robot_Out(hit_bot.bot_idx);
                        }
                    }

                    b_state = Ball_State.HIT;
                }
                else if (!is_ground) // if Obstacles or Ball
                {
                    b_state = Ball_State.HIT;
                }

                break;
        }
    }

    
}
