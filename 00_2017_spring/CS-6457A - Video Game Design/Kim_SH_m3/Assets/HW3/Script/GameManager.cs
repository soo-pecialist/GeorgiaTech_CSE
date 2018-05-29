using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class GameManager : MonoBehaviour {

    public enum Sound_Type
    {
        BALL_RESPAWN,
        BALL_SHOT,
        BOT_OUT,
        BOT_REVIVE,
        GET_BALL,
        GAMEOVER
    }

    public static GameManager instance;
    public int Stage_Size = 30;
    public int Ball_Speed = 30;
    public int Bot_Number_Per_Team = 2;
    public int respawn_idx;
    public float t;
    public float respawn_time;

    public Collider[] Borders;
    public Transform[] NavPoints;
    public List<GameObject> ChasingObj_List = new List<GameObject>();
    public List<bool> ChasingObj_check = new List<bool>();
    public Transform[] Obs;
    public List<Transform> Obs_Bump = new List<Transform>();

    public List<Ball> Ball_List = new List<Ball>();
    public RespawnArea[] Respawn_List = new RespawnArea[4];
    public List<Robot> Bot_List = new List<Robot>();

    public AudioClip[] Audios;

    public Ball pf_Ball;
    public GamePlayer pf_Player;
    public GameAIBot pf_Bot;

    void Awake()
    {
        if (instance == null)
            instance = this;
    }

	// Use this for initialization
	void Start () {
        for (int i = 0; i < Obs.Length; ++i)
        {

            Transform temp = new GameObject().transform;
            temp.gameObject.transform.parent = this.transform;
            temp.gameObject.transform.name = "Obs_Bump" + i;

            Vector3 temp_pos = Obs[i].localPosition;
            float ang = Obs[i].transform.localEulerAngles.y;
            float x_bump = 2 * Mathf.Cos(ang * Mathf.Deg2Rad);
            float z_bump = 2 * Mathf.Sin(ang * Mathf.Deg2Rad);
            temp_pos.x += temp_pos.x > 0 ? x_bump : -x_bump;
            temp_pos.z += temp_pos.x > 0 ? -z_bump : z_bump;
            temp_pos.y = 0;
            temp.localPosition = temp_pos;
            Obs_Bump.Add(temp);
        }

        respawn_time = UnityEngine.Random.Range(1f, 3f);
	}
	
	// Update is called once per frame
	void Update () {

        if (!is_playing)
        {
            statescore.text = "";
            return;
        }

        Set_StateScore();

        t += Time.deltaTime;
        if (t > respawn_time)
        {
            t = 0;
            respawn_time = UnityEngine.Random.Range(1f, 3f);
            Ball_Respawn();
        }

        

        #region TEST_CASE
        if (Input.GetKeyDown(KeyCode.O))
            AIBot_Respawn();

        if (Input.GetKeyDown(KeyCode.P))
            Player_Respawn();

        if (Input.GetKeyDown(KeyCode.R))
            Ball_Respawn();
        #endregion

    }

    #region ROBOT
    private void AIBot_Respawn()
    {
        int bot_index = Bot_List.Count - 1;
        for (int i = 0; i < Bot_Number_Per_Team * 2; ++i)
        {
            GameAIBot obBot = null;
            bot_index += 1;

            obBot = GameObject.Instantiate(pf_Bot) as GameAIBot;
            obBot.bot_idx = bot_index;
            obBot.Set_Team(i < Bot_Number_Per_Team);

            // set position
            int gap = 4;
            Vector3 bot_pos = Vector3.right * 12;
            if (Bot_Number_Per_Team % 2 == 0)
            {
                bot_pos.z = -gap*(Bot_Number_Per_Team/2f-1f) - (gap/2f) + gap*(i % Bot_Number_Per_Team);
                if (i < Bot_Number_Per_Team) bot_pos.x *= -1;
            }
            else
            {
                bot_pos.z = -gap * (Bot_Number_Per_Team - 1) / 2f + gap * (i % Bot_Number_Per_Team);
                if (i < Bot_Number_Per_Team) bot_pos.x *= -1;
            }
            
            obBot.gameObject.transform.localPosition = bot_pos;
            obBot.Play_Style = (GameAIBot.AIStyle)UnityEngine.Random.Range(0,2);
            Bot_List.Add(obBot);
        }
    }

    private void Player_Respawn()
    {
        GamePlayer obPlayer = null;
        int bot_index = Bot_List.Count;

        obPlayer = GameObject.Instantiate(pf_Player) as GamePlayer;
        obPlayer.bot_idx = bot_index;
        
        // set position
        Vector3 bot_pos = Vector3.zero;
        bot_pos.x = -12;
        obPlayer.gameObject.transform.localPosition = bot_pos;
        Bot_List.Add(obPlayer);
    }

    public void Robot_Out(int bot_idx)
    {
        // 1. 위 아래 중 가까운 곳 지정
        Robot Target = Bot_List[bot_idx];
        int nav_idx = 0;
        
        if (Target.is_left_team)
        {
            nav_idx = (Target.transform.localPosition.z < 0) ? 1 : 5;
        }
        else
        {
            nav_idx = (Target.transform.localPosition.z < 0) ? 13 : 9;
        }

        Transform[] NavArray = new Transform[3];
        for (int i = 0; i < 3; ++i)
            NavArray[i] = NavPoints[nav_idx + i];

        Target.Get_Out(NavArray);

        if (Check_GameEnd()) //// 게임 끝!!!
        {
            
            return;
        }

        PlaySound(Sound_Type.BOT_OUT);

        NOTI_Out(bot_idx);
    }

    public void Robot_Revive(int bot_idx)
    {
        Robot Target = Bot_List[bot_idx];
        int nav_idx = 0;

        if (Target.is_left_team)
        {
            nav_idx = (Target.transform.localPosition.z < 0) ? 3 : 7;
        }
        else
        {
            nav_idx = (Target.transform.localPosition.z < 0) ? 15 : 11;
        }

        Transform[] NavArray = new Transform[4];
        for (int i = 0; i < 4; ++i)
            NavArray[i] = NavPoints[nav_idx - i];

        Bot_List[bot_idx].Revive(NavArray);

        PlaySound(Sound_Type.BOT_REVIVE);

        NOTI_Revive(bot_idx);
    }

    #endregion

    #region BALL
    private void Ball_Respawn(int res_idx = -1)
    {
        bool all_occupied = true;
        for(int i = 0; i < Respawn_List.Length; ++i)
        {
            all_occupied &= Respawn_List[i].is_occupied();
        }
        if(all_occupied)
        {
            Debug.Log("All Respawn Points Are Occupied");
            return;
        }

        Ball obBall = null;

        if (Ball_List.Count == 0) // first time generate
        {
            obBall = GameObject.Instantiate(pf_Ball) as Ball;
            obBall.b_idx = 0;
            Ball_List.Add(obBall);
        }
        else
        {
            for(int i = 0; i < Ball_List.Count; ++i)
            {
                if (!Ball_List[i].is_busy)
                {
                    obBall = Ball_List[i];
                    break;
                }
            }

            if (obBall == null)
            {
                obBall = GameObject.Instantiate(pf_Ball) as Ball;
                obBall.b_idx = Ball_List.Count;
                Ball_List.Add(obBall);
            }
        }

        // Respown Position

        if (res_idx == -1)
        {
            do
            {
                respawn_idx = UnityEngine.Random.Range(0, Respawn_List.Length);
            } while (Respawn_List[respawn_idx].is_occupied());
        }
        else
            respawn_idx = res_idx;

        Debug.Log(respawn_idx + "th is Activated");
        obBall.res_idx = respawn_idx;
        Respawn_List[respawn_idx].Activate();

        Vector3 res_pos = Respawn_List[respawn_idx].gameObject.transform.localPosition;
        res_pos.y = 0.5f;
        obBall.transform.localPosition = res_pos;
        obBall.GetComponent<Rigidbody>().isKinematic = true;
        obBall.B_State = Ball.Ball_State.WAIT;

        Debug.Log("Respown");
        PlaySound(Sound_Type.BALL_RESPAWN);


        // Notify
        NOTI_BallRespawn(respawn_idx, res_pos);
    }

    public void Ball_Store(int b_idx, int bot_idx)
    {
        // set ball pos, state
        Ball target_ball = Ball_List[b_idx];

        target_ball.transform.localPosition = new Vector3(Stage_Size, 0, Stage_Size);
        target_ball.GetComponent<Rigidbody>().isKinematic = true;
        target_ball.B_State = Ball.Ball_State.POSSESED;
        target_ball.is_leftteam = Bot_List[bot_idx].is_left_team;

        Respawn_List[target_ball.res_idx].Deactivate();
        Debug.Log("Ball Stored");

        PlaySound(Sound_Type.GET_BALL);

        // set bot state
        Bot_List[bot_idx].Obtain_Ball(b_idx);

        // Notify
        NOTI_BallStore(b_idx, bot_idx);   
    }

    public void Ball_Shot(int b_idx, Vector3 b_pos, Vector3 b_rot)
    {
        Ball target_ball = Ball_List[b_idx];

        if (target_ball.B_State != Ball.Ball_State.POSSESED)
            return;

        Vector3 dir = new Vector3(Mathf.Sin(b_rot.y * Mathf.Deg2Rad), 0, Mathf.Cos(b_rot.y * Mathf.Deg2Rad));
        target_ball.transform.localPosition = b_pos + dir*1.5f + Vector3.up * 0.5f;
        target_ball.transform.localEulerAngles = b_rot;
        target_ball.GetComponent<Rigidbody>().isKinematic = false;
        target_ball.GetComponent<Rigidbody>().velocity = dir * Ball_Speed;
        target_ball.B_State = Ball.Ball_State.SHOT;
        Debug.Log("Ball Shot");

        PlaySound(Sound_Type.BALL_SHOT);

        // Notify
        NOTI_BallShot();
    }

    public void Ball_InActive(int b_idx)
    {
        Ball target_ball = Ball_List[b_idx];

        target_ball.transform.localPosition = new Vector3(Stage_Size, 0, Stage_Size);
        target_ball.GetComponent<Rigidbody>().isKinematic = true;
        target_ball.B_State = Ball.Ball_State.INACTIVE;
        Respawn_List[target_ball.res_idx].Deactivate();

        Debug.Log("Ball InActivated");
    }
    #endregion

    #region NOTIFY

    public void NOTI_BallRespawn(int res_idx, Vector3 res_pos)
    {
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot aibot = Bot_List[i].GetComponent<GameAIBot>();
            if (aibot != null)
                aibot.MSG_BallRespawn(res_idx, res_pos);
        }
    }

    public void NOTI_BallStore(int ball_index, int bot_index)
    {
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot aibot = Bot_List[i].GetComponent<GameAIBot>();
            if (aibot != null)
                aibot.MSG_BallStore(bot_index);
        }
    }

    public void NOTI_BallShot()
    {
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot aibot = Bot_List[i].GetComponent<GameAIBot>();
            if (aibot != null)
                aibot.MSG_BallShot();
        }
    }

    public void NOTI_Out(int bot_idx)
    {
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot t = Bot_List[i].GetComponent<GameAIBot>();
            if (t == null) continue;
            t.MSG_Out(bot_idx);
        }
    }

    public void NOTI_Revive(int bot_idx)
    {
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot t = Bot_List[i].GetComponent<GameAIBot>();
            if (t == null) continue;
            t.MSG_Revive(bot_idx);
        }
    }

    #endregion


    #region TEAM Strategy
    public int FindBot_GetBall(bool is_left, int res_idx, Vector3 ball_pos)
    {
        int target_bot_idx = -1;
        float dis = 1000;
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot AIBot = Bot_List[i].GetComponent<GameAIBot>();
            if (AIBot == null || AIBot.is_left_team != is_left || AIBot.has_ball) // if player or not possible
                continue;
            if (AIBot.bot_area != Robot.Bot_Area.INGAME) continue;

            Vector3 test_pos = AIBot.transform.localPosition;
            float d = Vector3.Distance(test_pos, ball_pos);
            if (d < dis)
            {
                dis = d;
                target_bot_idx = AIBot.bot_idx;
            }
        }

        if (target_bot_idx < 0)
        {
            Debug.Log("Not Exist Proper Bot");
            return target_bot_idx;
        }

        // Order
        Debug.Log(target_bot_idx + "th Bot Heading To Get Ball");
        Bot_List[target_bot_idx].GetComponent<GameAIBot>().HeadingToGetBall(res_idx);

        // Order to free bot
        //for (int i = 0; i < Bot_List.Count; ++i)
        //{
        //    GameAIBot AIBot = Bot_List[i].GetComponent<GameAIBot>();
        //    if (AIBot == null) continue;
        //    if (AIBot.bot_area != Robot.Bot_Area.INGAME) continue;
        //    if (AIBot.is_left_team != is_left) continue;
        //    if (AIBot.has_ball || AIBot.assigned_respawn > -1) continue;

        //    if (i == target_bot_idx)
        //        AIBot.HeadingToGetBall(res_idx);
        //    else
        //        AIBot.CallBack_FindBot_GetBall();
        //}

        return target_bot_idx;
    }

    public void FindBot_Disturb_Revive(bool is_left)
    {
        List<int> marked_prisoners = new List<int>();
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot bot = Bot_List[i].GetComponent<GameAIBot>();
            if (bot == null) continue;
            if (bot.bot_area != Robot.Bot_Area.INGAME) continue;
            if (bot.is_left_team != is_left) continue;
            if (bot.Team_State == GameAIBot.TeamState.LL || bot.Team_State == GameAIBot.TeamState.LM) continue;
            if (bot.chase_bot_idx == -1) continue;
            marked_prisoners.Add(bot.chase_bot_idx);
        }
        /// 1. 죄수 파악
        List<Robot> new_prisoners = new List<Robot>();
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            Robot bot = GameManager.instance.Bot_List[i];
            if (is_left != bot.is_left_team)
                if (bot.bot_area == Robot.Bot_Area.PRISON)
                    if (!marked_prisoners.Contains(bot.bot_idx))
                        new_prisoners.Add(bot);
        }

        /// 2. 가까운 죄수 할당 (ofensive 인 애들중 XO)
        for (int i = 0; i < new_prisoners.Count; ++i)
        {
            Vector3 prisoner_pos = new_prisoners[i].transform.localPosition;
            float dis = 1000;
            int target_idx = -1;
            for (int j = 0; j < Bot_List.Count; ++j)
            {
                GameAIBot bot = Bot_List[j].GetComponent<GameAIBot>();
                if (bot == null) continue;
                if (bot.bot_area != Robot.Bot_Area.INGAME) continue;
                if (bot.is_left_team != is_left || bot.Play_Style == GameAIBot.AIStyle.DEFENSIVE) continue;
                if (bot.Team_State == GameAIBot.TeamState.LL || bot.Team_State == GameAIBot.TeamState.LM) continue;
                if (bot.chase_bot_idx != -1) continue;
                if (bot.chase_bot_idx == new_prisoners[i].bot_idx) { target_idx = -1; break; }

                float d = Vector3.Distance(prisoner_pos, bot.transform.localPosition);
                if (d < dis)
                {
                    dis = d;
                    target_idx = j;
                }
            }

            if (target_idx != -1)
            {
                Bot_List[target_idx].GetComponent<GameAIBot>().chase_bot_idx = new_prisoners[i].bot_idx;
            }
        }

        /// 3. 추적 시작
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot bot = Bot_List[i].GetComponent<GameAIBot>();
            if (bot == null) continue;
            if (bot.bot_area != Robot.Bot_Area.INGAME) continue;
            if (bot.is_left_team != is_left || bot.Play_Style == GameAIBot.AIStyle.DEFENSIVE) continue;
            if (bot.Team_State == GameAIBot.TeamState.LL || bot.Team_State == GameAIBot.TeamState.LM) continue;

            bot.GoDisturb_Revive();
        }
    }

    public void FindRevive_TeamMate(bool is_left)
    {
        /// 1. 이미 할당 된 죄수 파악
        List<int> marked_prisoners = new List<int>();
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot bot = Bot_List[i].GetComponent<GameAIBot>();
            if (bot == null) continue;
            if (bot.bot_area != Robot.Bot_Area.INGAME) continue;
            if (bot.is_left_team != is_left) continue;
            //if (bot.Team_State == GameAIBot.TeamState.ML || bot.Team_State == GameAIBot.TeamState.MM) continue;
            if (bot.chase_bot_idx == -1) continue;
            marked_prisoners.Add(bot.chase_bot_idx);
        }

        /// 2. 새로 들어온 죄수 파악
        List<Robot> new_prisoners = new List<Robot>();
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            Robot bot = GameManager.instance.Bot_List[i];
            if (is_left == bot.is_left_team)
                if (bot.bot_area == Robot.Bot_Area.PRISON)
                    if (!marked_prisoners.Contains(bot.bot_idx))
                        new_prisoners.Add(bot);
        }

        /// 2. 가까운 팀원죄수 할당 (공을 가지고 있는 애, OX or OO)
        for (int i = 0; i < new_prisoners.Count; ++i)
        {
            Vector3 prisoner_pos = new_prisoners[i].transform.localPosition;
            float dis = 1000;
            int target_idx = -1;
            for (int j = 0; j < Bot_List.Count; ++j)
            {
                GameAIBot bot = Bot_List[j].GetComponent<GameAIBot>();
                if (bot == null) continue;
                if (bot.bot_area != Robot.Bot_Area.INGAME) continue;
                if (bot.is_left_team != is_left) continue;
                if (!bot.has_ball) continue;
                if (bot.Team_State == GameAIBot.TeamState.LL && bot.Play_Style == GameAIBot.AIStyle.OFFENSIVE) continue;
                if (bot.chase_bot_idx != -1) continue;
                if (bot.chase_bot_idx == new_prisoners[i].bot_idx) { target_idx = -1; break; }

                float d = Vector3.Distance(prisoner_pos, bot.transform.localPosition);
                if (d < dis)
                {
                    dis = d;
                    target_idx = j;
                }
            }

            if (target_idx != -1)
            {
                Bot_List[target_idx].GetComponent<GameAIBot>().chase_bot_idx = new_prisoners[i].bot_idx;
            }
        }

        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot bot = Bot_List[i].GetComponent<GameAIBot>();
            if (bot == null) continue;
            if (bot.bot_area != Robot.Bot_Area.INGAME) continue;
            if (bot.is_left_team != is_left) continue;
            if (!bot.has_ball) continue;
            if (bot.Team_State == GameAIBot.TeamState.LL && bot.Play_Style == GameAIBot.AIStyle.OFFENSIVE) continue;
            if (bot.Team_State == GameAIBot.TeamState.ML || bot.Team_State == GameAIBot.TeamState.MM) continue;

            bot.GoRevive_Teammate();
        }

        return;
    }

    public void Update_TeamState()
    {
        int num_left = 0, num_right = 0;
        for (int i = 0; i < Bot_List.Count; ++i)
        {
            Robot t = Bot_List[i];
            if (t.bot_area != Robot.Bot_Area.INGAME) continue;
            if (t.is_left_team) num_left++;
            else num_right++;
        }

        for (int i = 0; i < Bot_List.Count; ++i)
        {
            GameAIBot t = Bot_List[i].GetComponent<GameAIBot>();
            if (t == null) continue;
            t.CallBack_Update_TeamState(num_left, num_right);
        }
    }

    #endregion

    #region GAME MANAGE
    public bool is_playing = false;
    public void GameStart()
    {
        is_playing = true;
        result.gameObject.SetActive(false);

        ResetGame();

        AIBot_Respawn();
    }

    public void ResetGame()
    {
        for (int i = 0; i < Bot_List.Count; ++i)
            DestroyObject(Bot_List[i].gameObject);
        Bot_List.Clear();

        for (int i = 0; i < Ball_List.Count; ++i)
            DestroyObject(Ball_List[i].gameObject);
        Ball_List.Clear();

        for (int i = 0; i < Respawn_List.Length; ++i)
            Respawn_List[i].Deactivate();
    }

    public bool Check_GameEnd()
    {
        bool left_alive = false;
        bool right_alive = false;

        for (int i = 0; i < Bot_List.Count; ++i)
        {
            Robot t = Bot_List[i];
            if (t.is_left_team) left_alive |= (t.bot_area == Robot.Bot_Area.INGAME);
            else right_alive |= (t.bot_area == Robot.Bot_Area.INGAME);
        }

        if (!left_alive)
            result.text = "RIGHT TEAM WIN!!";
        else if (!right_alive)
            result.text = "LEFT TEAM WIN!!";

        bool is_end = left_alive != right_alive;

        if (is_end)
        {
            PlaySound(Sound_Type.GAMEOVER);
            is_playing = false;
            result.gameObject.SetActive(true);
        }
        return is_end;
    }

    public int Give_ChasingObject()
    {
        // find usuable object
        for (int i = 0; i < ChasingObj_check.Count; ++i)
        {
            if (!ChasingObj_check[i])
            {
                ChasingObj_check[i] = true;
                return i;
            }
        }

        // not exist object
        int idx = ChasingObj_check.Count;
        GameObject temp = new GameObject();
        ChasingObj_List.Add(temp);
        ChasingObj_check.Add(true);

        return idx;
    }

    public void Return_ChasingObject(int chasing_idx)
    {
        if (chasing_idx == -1) return;
        ChasingObj_check[chasing_idx] = false;
    }

    public void PlaySound(GameManager.Sound_Type type)
    {
        GameObject t = new GameObject();
        AudioSource s = t.AddComponent<AudioSource>();
        s.PlayOneShot(Audios[(int)type]);
        DestroyObject(t, 2);
    }


    public InputField Inputfield;
    public void Set_NumberOfPlayer()
    {
        Bot_Number_Per_Team = Convert.ToInt32(Inputfield.text);
    }

    public Text statescore;
    public Text result;
    public void Set_StateScore()
    {
        string t = "";

        for (int i = 0; i < Bot_List.Count; ++i)
        {
            Robot r = Bot_List[i];
            t += r.is_left_team ? "<T1> " : "<T2> ";
            if (r.GetComponent<GameAIBot>() == null)
                t += i + "th Player : ";
            else
                t += i + "th AI Bot : ";

            if (r.bot_area == Robot.Bot_Area.INGAME)
                t += "Playing\n";
            else if (r.bot_area == Robot.Bot_Area.PRISON)
                t += "In Prison\n";
            else if (r.bot_area == Robot.Bot_Area.SIDELINE)
                t += "SideLine\n";
        }

        statescore.text = t;
    }

    #endregion

}
