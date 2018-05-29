using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameAIBot : Robot
{
    public enum AIStyle
    {
        OFFENSIVE,
        DEFENSIVE
    }

    public enum StrState
    {
        XX,
        XO,
        OX,
        OO,
        NONE
    }

    public enum TeamState
    {
        LL,
        LM,
        ML,
        MM
    }

    public AIStyle Play_Style = AIStyle.OFFENSIVE;
    public StrState Cur_State = StrState.NONE;
    public TeamState Team_State = TeamState.MM;

    public int assigned_respawn = -1;
    public int chase_bot_idx = -1;
    public int chase_idx = -1;
    public Transform chase_target = null;

    protected override void Start()
    {
        base.Start();
        Transit_XX();
    }

    protected override void FixedUpdate()
    {
        base.FixedUpdate();

        if (bot_area == Bot_Area.INGAME)
        {
            switch (Cur_State)
            {
                case StrState.XX:
                    Action_XX();
                    break;
                case StrState.XO:
                    Action_XO();
                    break;
                case StrState.OX:
                    Action_OX();
                    break;
                case StrState.OO:
                    Action_OO();
                    break;
            }
        }
        else if (bot_area == Bot_Area.PRISON)
            Action_Prison();

#region TestCode
        //if (Input.GetButtonDown("Fire1"))
        //    Shot_Ball();
        #endregion

    }

    #region Action_State
    private void Action_XX()
    {
    }

    private void Action_XO()
    {
        if (chase_idx > -1)
        {
            chase_target.localPosition = get_chaser_position();
            AINav.setWayPoint(chase_target);
        }
    }

    private void Action_OX()
    {
        switch (Team_State)
        {
            case TeamState.LL:
                if (Play_Style == AIStyle.OFFENSIVE)
                    goto case TeamState.MM;
                else
                    goto case TeamState.LM;
            case TeamState.LM: ///// Teammate Revive Action
                chase_target.transform.localPosition = GameManager.instance.Bot_List[chase_bot_idx].transform.localPosition;
                AINav.setWayPoint(chase_target);

                Vector3 dir0 = transform.TransformDirection(Vector3.forward);
                RaycastHit hitInfo0 = new RaycastHit();
                Debug.DrawRay(transform.position, dir0, Color.green, 1);

                if (Physics.SphereCast(transform.position, 0.5f, dir0, out hitInfo0))
                {
                    if (hitInfo0.transform.CompareTag("Player"))
                    {
                        if (hitInfo0.transform.GetComponent<Robot>().is_left_team != is_left_team)
                            break;

                        Debug.DrawRay(hitInfo0.point, hitInfo0.normal, Color.red, 1);

                        Shot_Ball();
                        chase_bot_idx = -1;
                        GameManager.instance.Return_ChasingObject(chase_idx);
                        chase_idx = -1;
                        //AINav.clearWaypoints();
                    }
                }
                break;
            case TeamState.ML:
            case TeamState.MM: ////// Enemy Kill Action
                {
                    chase_target.transform.localPosition = GameManager.instance.Bot_List[chase_bot_idx].transform.localPosition;
                    AINav.setWayPoint(chase_target);
                    
                    Vector3 dir1 = transform.TransformDirection(Vector3.forward);
                    RaycastHit hitInfo1 = new RaycastHit();
                    Debug.DrawRay(transform.position, dir1, Color.yellow, 1);

                    if (Physics.SphereCast(transform.position, 0.5f, dir1, out hitInfo1))
                    {
                        Debug.DrawRay(hitInfo1.point, hitInfo1.normal, Color.red, 1);
                        if (hitInfo1.transform.CompareTag("Player"))
                        {
                            if (hitInfo1.transform.GetComponent<Robot>().is_left_team == is_left_team) break;
                            if (hitInfo1.transform.GetComponent<Robot>().bot_area != Bot_Area.INGAME) break;

                            Shot_Ball();
                            chase_bot_idx = -1;
                            GameManager.instance.Return_ChasingObject(chase_idx);
                            chase_idx = -1;
                            //AINav.clearWaypoints();
                        }
                        else if (hitInfo1.transform.CompareTag("Obs"))
                        {
                            // 가만히 숨어 있는 경우
                            //if (Play_Style == AIStyle.OFFENSIVE)
                            //{
                                if (exist_prisoner(true))
                                    FindRevive_Teammate();
                                else
                                {
                                    Vector3 dir = GameManager.instance.Bot_List[chase_bot_idx].transform.localPosition - transform.localPosition;
                                    Vector3 move_dir = transform.localPosition;
                                    move_dir.z += (dir.z > 0) ? 1 : -1;
                                    Mathf.Clamp(move_dir.z, -GameManager.instance.Stage_Size / 2, GameManager.instance.Stage_Size / 2);
                                    chase_target.transform.localPosition = move_dir;
                                    AINav.setWayPoint(chase_target);
                                    break; //////////// 공격 가능한 위치로 이동!! 만들어야됨
                                }
                            //}
                            //else
                            //    Transit_OX();
                            break; ////// Defensive 일 땐 대기
                        }
                    }
                }
                break;
        }
    }

    private void Action_OO()
    {
        switch (Team_State)
        {
            case TeamState.LL:
                if (Play_Style == AIStyle.OFFENSIVE)
                    goto case TeamState.MM;
                else
                    goto case TeamState.LM;
            case TeamState.LM: ///// Teammate Revive Action
                chase_target.transform.localPosition = GameManager.instance.Bot_List[chase_bot_idx].transform.localPosition;
                AINav.setWayPoint(chase_target);

                Vector3 dir0 = transform.TransformDirection(Vector3.forward);
                RaycastHit hitInfo0 = new RaycastHit();
                Debug.DrawRay(transform.position, dir0, Color.blue, 1);

                if (Physics.SphereCast(transform.position, 0.5f, dir0, out hitInfo0))
                {
                    if (hitInfo0.transform.CompareTag("Player"))
                    {
                        if (hitInfo0.transform.GetComponent<Robot>().is_left_team != is_left_team)
                            break;

                        Debug.DrawRay(hitInfo0.point, hitInfo0.normal, Color.red, 1);

                        Shot_Ball();
                        chase_bot_idx = -1;
                        GameManager.instance.Return_ChasingObject(chase_idx);
                        chase_idx = -1;
                        //AINav.clearWaypoints();
                    }
                }
                break;
            case TeamState.ML:
            case TeamState.MM: ////// Enemy Kill Action
                {
                    chase_target.transform.localPosition = GameManager.instance.Bot_List[chase_bot_idx].transform.localPosition;
                    AINav.setWayPoint(chase_target);

                    Vector3 dir1 = transform.TransformDirection(Vector3.forward);
                    RaycastHit hitInfo1 = new RaycastHit();
                    Debug.DrawRay(transform.position, dir1, Color.blue, 1);

                    if (Physics.SphereCast(transform.position, 0.5f, dir1, out hitInfo1))
                    {
                        Debug.DrawRay(hitInfo1.point, hitInfo1.normal, Color.red, 1);
                        if (hitInfo1.transform.CompareTag("Player"))
                        {
                            if (hitInfo1.transform.GetComponent<Robot>().is_left_team == is_left_team) break;
                            if (hitInfo1.transform.GetComponent<Robot>().bot_area != Bot_Area.INGAME) break;

                            Shot_Ball();
                            chase_bot_idx = -1;
                            GameManager.instance.Return_ChasingObject(chase_idx);
                            chase_idx = -1;
                            //AINav.clearWaypoints();
                        }
                    }
                }
                break;
        }
    }
    
    private void Action_Prison()
    {
        if (chase_bot_idx == -1)
            return;

        Vector3 dir = GameManager.instance.Bot_List[chase_bot_idx].transform.localPosition - transform.localPosition;
        RaycastHit hitInfo = new RaycastHit();
        Debug.DrawRay(transform.position, dir, Color.black, 1);
        
        if (Physics.SphereCast(transform.position, 0.5f, dir, out hitInfo))
        {
            Vector3 move_dir = transform.localPosition;
            if (hitInfo.transform.CompareTag("Player"))
            {
                Debug.DrawRay(hitInfo.point, hitInfo.normal, Color.red, 1);
                Robot t = hitInfo.transform.GetComponent<Robot>();
                if (t.is_left_team != is_left_team) return;
                if (t.bot_idx != chase_bot_idx) return;
            }
            else
            {
                move_dir.z += (dir.z > 0) ? 1: -1;
                Mathf.Clamp(move_dir.z, -GameManager.instance.Stage_Size / 2, GameManager.instance.Stage_Size / 2);
            }
            chase_target.transform.localPosition = move_dir;
            AINav.setWayPoint(chase_target);
        }
    }

    #endregion

    #region Transit_State
    private void Transit_XX()
    {
        Cur_State = StrState.XX;

        if (bot_area != Bot_Area.INGAME) return;
        if (assigned_respawn != -1) return;

        switch (Play_Style)
        {
            case AIStyle.OFFENSIVE:
                Patrol_Respawn();
                break;
            case AIStyle.DEFENSIVE:
                Hide_ToObs();
                break;
        }
    }

    private void Transit_XO()
    {
        Cur_State = StrState.XO;

        if (bot_area != Bot_Area.INGAME) return;
        if (assigned_respawn != -1) return; // 먹으러 가던 공이 있으면 먹으러 감

        switch (Team_State)
        {
            case TeamState.LL:
            case TeamState.LM:
                {
                    if (Play_Style == AIStyle.OFFENSIVE)
                        Patrol_Obs();
                    else
                        Hide_ToObs();
                }
                break;
            case TeamState.ML:
            case TeamState.MM:
                {
                    if (Play_Style == AIStyle.OFFENSIVE)
                    {
                        if (exist_prisoner(false)) // exist enemy prisoner
                            FindBot_Disturb_Revive();
                        else
                            Patrol_Obs();
                    }
                    else
                        Patrol_Obs();
                }
                break;
        }
    }

    private void Transit_OX()
    {
        Cur_State = StrState.OX;
        
        switch (Team_State)
        {
            case TeamState.LL:
                {
                    if (Play_Style == AIStyle.OFFENSIVE)
                        FindKill_Enemy();
                    else
                        FindRevive_Teammate();
                }
                break;
            case TeamState.LM:
                {
                    FindRevive_Teammate();
                }
                break;
            case TeamState.ML:
            case TeamState.MM:
                {
                    FindKill_Enemy();
                }
                break;
        }
    }

    private void Transit_OO()
    {
        Cur_State = StrState.OO;

        switch (Team_State)
        {
            case TeamState.LL:
                {
                    if (Play_Style == AIStyle.OFFENSIVE)
                        FindKill_Enemy();
                    else
                        FindRevive_Teammate();
                }
                break;
            case TeamState.LM:
                {
                    FindRevive_Teammate();
                }
                break;
            case TeamState.ML:
            case TeamState.MM:
                {
                    FindKill_Enemy();
                }
                break;
        }
    }

    private void Transit_Prison(int bot_idx)
    {
        // 친구 찾아갈 준비
        chase_bot_idx = bot_idx;

        chase_idx = GameManager.instance.Give_ChasingObject();
        chase_target = GameManager.instance.ChasingObj_List[chase_idx].transform;
    }

    #endregion

    #region Message

    public void MSG_BallRespawn(int res_idx, Vector3 res_pos)
    {
        if (bot_area != Bot_Area.INGAME) return;

        switch (Cur_State)
        {
            case StrState.XX:
                {
                    if (can_get_ball(res_pos))
                    {
                        FindBot_GetBall(res_idx, res_pos);
                    }
                    else
                    {
                        if (assigned_respawn == -1) // 공 주으러 가는 중이 아니면 숨음
                            Hide_ToObs();
                    }
                }
                break;
            case StrState.XO:
                {
                    switch (Team_State)
                    {
                        case TeamState.LL:
                        case TeamState.LM:
                            {
                                if (can_get_ball(res_pos) && is_full_respawn())
                                    FindBot_GetBall(res_idx, res_pos);
                                else
                                    Patrol_Obs();
                            }
                                break;
                        case TeamState.ML:
                        case TeamState.MM:
                            {
                                int tb_idx = -1;
                                if (can_get_ball(res_pos))
                                    tb_idx = FindBot_GetBall(res_idx, res_pos);
                                if (tb_idx != bot_idx && tb_idx != -1)
                                {
                                    Patrol_Obs();
                                    if (Play_Style == AIStyle.OFFENSIVE && exist_prisoner(false))
                                        FindBot_Disturb_Revive();
                                }
                            }
                            break;
                    }
                }
                break;
            case StrState.OX:
                Transit_OX();
                break;
            case StrState.OO:
                Transit_OO();
                break;
        }
    }

    public void MSG_BallStore(int get_bot_idx)
    {
        if (bot_area != Bot_Area.INGAME) return;

        Robot t = GameManager.instance.Bot_List[get_bot_idx];
        if (get_bot_idx == bot_idx) // i got ball
        {
            if (Cur_State == StrState.XX) Transit_OX();
            else if (Cur_State == StrState.XO) Transit_OO();
        }
        else if (is_left_team != t.is_left_team) // enemy got ball
        {
            if (Cur_State == StrState.XX || Cur_State == StrState.XO) Transit_XO();
            else if (Cur_State == StrState.OX || Cur_State == StrState.OO) Transit_OO();
        }
    }

    public void MSG_BallShot()
    {
        if (enemy_has_ball()) // 적이 공을 아직 가지고 있으면 현 상태 유지
            return;

        // 적이 공이 없으면 State 변경
        if (Cur_State == StrState.XO) Transit_XX();
        else if (Cur_State == StrState.OO) Transit_OX();
    }

    public void MSG_Out(int bot_index)
    {
        if (chase_bot_idx == bot_index)
        {
            chase_bot_idx = -1;
            GameManager.instance.Return_ChasingObject(chase_idx);
            chase_idx = -1;

            if (Cur_State == StrState.OX) Transit_OX();
            else if (Cur_State == StrState.OO) Transit_OO();
            else if (bot_area == Bot_Area.PRISON) Patrol_Prison();
        }
    }

    public void MSG_Revive(int bot_index)
    {
        if (chase_bot_idx == bot_index)
        {
            chase_bot_idx = -1;
            GameManager.instance.Return_ChasingObject(chase_idx);
            chase_idx = -1;
        }
    }

    #endregion

    #region Orders

    #region BALL_Order

    public override void Obtain_Ball(int b_idx)
    {
        base.Obtain_Ball(b_idx);

        assigned_respawn = -1;
        switch (Cur_State)
        {
            case StrState.XX:
                Transit_OX();
                
                break;
            case StrState.XO:
                Transit_OO();
                break;
            case StrState.OX:
            case StrState.OO:
                Debug.Log("OBTAIN BALL IS IMPOSSIBLE!!!");
                break;
        }
    }

    public override void Shot_Ball()
    {
        switch (Cur_State)
        {
            case StrState.XX:
            case StrState.XO:
                Debug.Log("OBTAIN BALL IS IMPOSSIBLE!!!");
                break;
            case StrState.OX:
                Transit_XX();
                break;
            case StrState.OO:
                Transit_XO();
                break;

        }

        base.Shot_Ball();
    }

    #endregion

    #region Bot Order
    public int FindBot_GetBall(int res_idx, Vector3 res_pos)
    {
        return GameManager.instance.FindBot_GetBall(is_left_team, res_idx, res_pos);
    }

    public void HeadingToGetBall(int res_idx)
    {
        Transform des = GameManager.instance.Respawn_List[res_idx].transform;
        AINav.setWayPoint(des);
        AINav.waypointLoop = false;
        assigned_respawn = res_idx;
    }

    public void Hide_ToObs()
    {
        Transform des = find_nearest_obs();
        if (des == null)
        {
            Debug.LogError("WHY DESTINATION IS NULL????????");
            return;
        }
        AINav.setWayPoint(des);
        AINav.waypointLoop = false;
    }

    public void Patrol_Respawn()
    {
        Transform[] des = find_patrol_respawn();
        AINav.setWayPoints(des);
        AINav.waypointLoop = true;
    }

    public void Patrol_Obs()
    {
        Transform[] des = find_patrol_obs();
        AINav.setWayPoints(des);
        AINav.waypointLoop = true;
    }

    public void Patrol_Prison()
    {
        Transform[] des = new Transform[2];

        if (is_left_team)
        {
            des[0] = (transform.localPosition.z < 0) ? GameManager.instance.NavPoints[3].transform : GameManager.instance.NavPoints[7].transform;
            des[1] = (transform.localPosition.z < 0) ? GameManager.instance.NavPoints[7].transform : GameManager.instance.NavPoints[3].transform;
        }
        else
        {
            des[0] = (transform.localPosition.z < 0) ? GameManager.instance.NavPoints[15].transform : GameManager.instance.NavPoints[15].transform;
            des[1] = (transform.localPosition.z < 0) ? GameManager.instance.NavPoints[11].transform : GameManager.instance.NavPoints[11].transform;
        }
        AINav.setWayPoints(des);
        AINav.waypointLoop = true;
    }

    public void FindBot_Disturb_Revive()
    {
        GameManager.instance.FindBot_Disturb_Revive(is_left_team);
    }

    public void GoDisturb_Revive()
    {
        if (chase_bot_idx == -1) // 마킹 죄수를 할당 못받은 애들은 그냥 패트롤
        {
            Patrol_Obs();
            return;
        }

        Debug.Log(this.name + " Chase " + chase_bot_idx + "th Bot!!");

        if (chase_idx != -1)
            return;

        chase_idx = GameManager.instance.Give_ChasingObject();
        chase_target = GameManager.instance.ChasingObj_List[chase_idx].transform;
    }

    public void FindKill_Enemy()
    {
        chase_bot_idx = find_kill_enemy();
        if (chase_bot_idx == -1)
        {
            Debug.LogError("STRANGE??!?!?!?! NO EXISTS ENEMY!!");
            return;
        }

        Debug.Log(this.name + " Go Kill " + chase_bot_idx + "th Bot!!");

        if (chase_idx != -1)
            return;

        chase_idx = GameManager.instance.Give_ChasingObject();
        chase_target = GameManager.instance.ChasingObj_List[chase_idx].transform;
    }

    public void FindRevive_Teammate()
    {
        if (exist_prisoner(true))
            GameManager.instance.FindRevive_TeamMate(is_left_team);
        else
            FindKill_Enemy();
    }

    public void GoRevive_Teammate()
    {
        if (chase_bot_idx == -1) // 살릴애 할당 못받은 애들은 그냥 공격
        {
            FindKill_Enemy();
            return;
        }

        Debug.Log(this.name + " Save " + chase_bot_idx + "th Bot~~~~~~~~~~~~~");

        if (chase_idx != -1)
            return;

        chase_idx = GameManager.instance.Give_ChasingObject();
        chase_target = GameManager.instance.ChasingObj_List[chase_idx].transform;

        //// Notify To Prisoner
        GameAIBot t = GameManager.instance.Bot_List[chase_bot_idx].GetComponent<GameAIBot>();
        if (t == null) return; // If Player
        if (t.is_left_team != is_left_team) return; // enemy
        t.Transit_Prison(bot_idx);
    }

    public override void Get_Out(Transform[] Navs)
    {
        assigned_respawn = -1;
        chase_bot_idx = -1;
        if (chase_idx != -1)
            GameManager.instance.Return_ChasingObject(chase_idx);
        chase_idx = -1;

        base.Get_Out(Navs);
    }

    #endregion

    #endregion

    public override void CallBack_NavEnd()
    {
        if (bot_area == Bot_Area.SIDELINE && is_revive) // 부활했다가 인게임 돌아 왔을 때
        {
            GameManager.instance.Update_TeamState();

            if (enemy_has_ball())
                Transit_XO();
            else
                Transit_XX();
        }

        base.CallBack_NavEnd();

        if (bot_area == Bot_Area.PRISON && stuck_prison)
        {
            Patrol_Prison();
        }
    }

    public void CallBack_Update_TeamState(int num_left, int num_right)
    {
        int num_myteam = is_left_team ? num_left : num_right;
        int num_enemy = is_left_team ? num_right : num_left;

        float p_mytem = num_myteam / (float) GameManager.instance.Bot_Number_Per_Team;
        float p_enemy = num_enemy / (float)GameManager.instance.Bot_Number_Per_Team;

        if (Play_Style == AIStyle.OFFENSIVE)
        {
            if (p_mytem >= 0.25f && p_enemy >= 0.25f) Team_State = TeamState.MM;
            else if (p_mytem >= 0.25f && (p_enemy < 0.25f || num_enemy == 1)) Team_State = TeamState.ML;
            else if ((p_mytem < 0.25f || num_myteam == 1) && (p_enemy < 0.25f || num_enemy == 1)) Team_State = TeamState.LL;
            else Team_State = TeamState.LM;
        }
        else
        {
            if (p_mytem > 0.5f && p_enemy > 0.5f) Team_State = TeamState.MM;
            else if (p_mytem > 0.5f && (p_enemy <= 0.5f || num_enemy == 1)) Team_State = TeamState.ML;
            else if ((p_mytem <= 0.5f || num_myteam == 1) && (p_enemy <= 0.5f || num_enemy == 1)) Team_State = TeamState.LL;
            else Team_State = TeamState.LM;
        }
    }

    #region Util Functions
    private bool can_get_ball(Vector3 b_pos)
    {
        return b_pos.x < 0 == is_left_team;
    }

    private Transform find_nearest_obs()
    {
        Transform target = null;
        Vector3 cur_pos = gameObject.transform.localPosition;
        float distance = 1000;
        for (int i = 0; i < GameManager.instance.Obs_Bump.Count; ++i)
        {
            Vector3 test_pos = GameManager.instance.Obs_Bump[i].localPosition;
            if (is_left_team == test_pos.x > 0)
                continue;

            float d = Vector3.Distance(cur_pos, test_pos);
            if (d < distance)
            {
                distance = d;
                target = GameManager.instance.Obs_Bump[i];
            }
        }

        return target;
    }

    private Transform[] find_patrol_obs()
    {
        Transform[] target = new Transform[2];
        target[0] = find_nearest_obs();

        Vector3 cur_pos = gameObject.transform.localPosition;
        float distance = 1000;
        for (int i = 0; i < GameManager.instance.Obs_Bump.Count; ++i)
        {
            Vector3 test_pos = GameManager.instance.Obs_Bump[i].localPosition;
            if (is_left_team == test_pos.x > 0 || target[0] == GameManager.instance.Obs_Bump[i])
                continue;

            float d = Vector3.Distance(cur_pos, test_pos);
            if (d < distance)
            {
                distance = d;
                target[1] = GameManager.instance.Obs_Bump[i];
            }
        }


        return target;
    }

    private Transform find_nearest_respawn()
    {
        Transform target = null;
        Vector3 cur_pos = gameObject.transform.localPosition;
        float distance = 1000;
        for (int i = 0; i < GameManager.instance.Respawn_List.Length; ++i)
        {
            Vector3 test_pos = GameManager.instance.Respawn_List[i].transform.localPosition;
            if (is_left_team == test_pos.x > 0)
                continue;

            float d = Vector3.Distance(cur_pos, test_pos);
            if (d < distance)
            {
                distance = d;
                target = GameManager.instance.Respawn_List[i].transform;
            }
        }

        return target;
    }

    private Transform[] find_patrol_respawn()
    {
        Transform[] target = new Transform[2];
        target[0] = find_nearest_respawn();

        Vector3 cur_pos = gameObject.transform.localPosition;
        float distance = 1000;
        for (int i = 0; i < GameManager.instance.Respawn_List.Length; ++i)
        {
            Vector3 test_pos = GameManager.instance.Respawn_List[i].transform.localPosition;
            if (is_left_team == test_pos.x > 0 || target[0] == GameManager.instance.Respawn_List[i].transform)
                continue;

            float d = Vector3.Distance(cur_pos, test_pos);
            if (d < distance)
            {
                distance = d;
                target[1] = GameManager.instance.Respawn_List[i].transform;
            }
        }


        return target;
    }

    private bool exist_prisoner(bool find_teammate)
    {
        for (int i = 0; i < GameManager.instance.Bot_List.Count; ++i)
        {
            Robot bot = GameManager.instance.Bot_List[i];
            if (find_teammate == (is_left_team == bot.is_left_team))
                if (bot.bot_area == Bot_Area.PRISON)
                    return true;
        }

        return false;
    }

    private Vector3 get_chaser_position()
    {
        float allow_area = (GameManager.instance.Stage_Size / 2) - 3;
        Vector3 temp_nav = GameManager.instance.Bot_List[chase_bot_idx].transform.localPosition;

        temp_nav.x = Mathf.Clamp(temp_nav.x, -allow_area, allow_area);
        temp_nav.z = Mathf.Clamp(temp_nav.z, -allow_area, allow_area);

        return temp_nav;
    }

    private bool is_full_respawn()
    {
        bool check_left = GameManager.instance.Respawn_List[0].is_occupied() && GameManager.instance.Respawn_List[1].is_occupied();
        bool check_right = GameManager.instance.Respawn_List[2].is_occupied() && GameManager.instance.Respawn_List[3].is_occupied();
        return is_left_team? check_left : check_right;
    }

    private int find_kill_enemy()
    {
        // 안 숨어 있는 봇 찾기
        int target_idx = -1;
        float dis = 1000;
        for (int i = 0; i < GameManager.instance.Bot_List.Count; ++i)
        {
            Robot bot = GameManager.instance.Bot_List[i];
            if (bot.bot_area != Robot.Bot_Area.INGAME) continue;
            if (bot.is_left_team == is_left_team) continue;

            RaycastHit hitInfo = new RaycastHit();
            Vector3 dir = bot.transform.position - transform.position;
            Debug.DrawRay(transform.position, dir, Color.gray, 3);

            if (Physics.SphereCast(transform.position, 0.5f, dir, out hitInfo))
            {
                Debug.DrawRay(hitInfo.point, hitInfo.normal, Color.blue, 3);
                Debug.Log("Ray HIT INFO : " + hitInfo.transform.name);

                if (hitInfo.transform.CompareTag("Player"))
                {
                    if (hitInfo.distance < dis)
                    {
                        dis = hitInfo.distance;
                        target_idx = i;
                    }
                }
            }
        }
        if (target_idx != -1)
        {
            Debug.Log("GOT YOU!!!!!!!!!! " + target_idx + "th BOT!!");
            return target_idx;
        }

        /// 전부 숨어있을 경우
        dis = 1000;
        for (int i = 0; i < GameManager.instance.Bot_List.Count; ++i)
        {
            Robot bot = GameManager.instance.Bot_List[i];
            if (bot.bot_area != Robot.Bot_Area.INGAME) continue;
            if (bot.is_left_team == is_left_team) continue;

            float d = Vector3.Distance(this.transform.localPosition, bot.transform.localPosition);
            if (d < dis)
            {
                dis = d;
                target_idx = i;
            }
        }

        if (target_idx == -1)
            Debug.LogError("No Exist Enemy!!!!");

        return target_idx;
    }

    private bool enemy_has_ball()
    {
        for (int i = 0; i < GameManager.instance.Bot_List.Count; ++i)
        {
            Robot temp = GameManager.instance.Bot_List[i];
            if (temp.is_left_team == is_left_team) continue; // 우리팀 상관없음
            if (temp.has_ball)
                return true;
        }

        return false;
    }


    #endregion

}
