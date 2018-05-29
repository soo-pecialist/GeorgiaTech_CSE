using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GamePlayer : Robot {

    public bool can_contriol = true;

    protected override void FixedUpdate()
    {
        base.FixedUpdate();

        if (can_contriol)
        {
            float h = Input.GetAxis("Horizontal");              // setup h variable as our horizontal input axis
            float v = Input.GetAxis("Vertical");                // setup v variables as our vertical input axis
            anim.SetFloat("Speed", v);                          // set our animator's float parameter 'Speed' equal to the vertical input axis				
            anim.SetFloat("Direction", h);                      // set our animator's float parameter 'Direction' equal to the horizontal input axis		
            anim.speed = animSpeed;                             // set the speed of our animator to the public variable 'animSpeed'

            currentBaseState = anim.GetCurrentAnimatorStateInfo(0); // set our currentState variable to the current state of the Base Layer (0) of animation

            if (anim.layerCount == 2)
                layer2CurrentState = anim.GetCurrentAnimatorStateInfo(1);   // set our layer2CurrentState variable to the current state of the second Layer (1) of animation


            // Shot the ball
            if (Input.GetButtonDown("Fire1"))
                Shot_Ball();
        }
        
    }

    public override void Get_Out(Transform[] Navs)
    {
        can_contriol = false;

        base.Get_Out(Navs);
    }

    public override void CallBack_NavEnd()
    {
        AINav.clearWaypoints();
        AINav.useNavMeshPathPlanning = false;
        can_contriol = true;

        base.CallBack_NavEnd();
    }

    public override void Revive(Transform[] Navs)
    {
        can_contriol = false;

        base.Revive(Navs);
    }
}
