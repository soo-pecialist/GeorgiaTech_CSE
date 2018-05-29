using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RespawnArea : MonoBehaviour {

    private bool is_respawning = false;
    private bool is_brightning = false;
    
    private Color temp_color;

    // Use this for initialization
    void Awake () {
        temp_color = this.gameObject.GetComponent<MeshRenderer>().material.color;
    }
	
	// Update is called once per frame
	void Update () {
        if (is_respawning)
        {
            if (is_brightning)
                temp_color.a += Time.deltaTime;
            else
                temp_color.a -= Time.deltaTime;

            if (temp_color.a >= 0.95f)
                is_brightning = false;
            else if (temp_color.a < 0.3f)
                is_brightning = true;

            this.gameObject.GetComponent<MeshRenderer>().material.color = temp_color;
        }
    }

    public void Activate()
    {
        temp_color.a = 0.5f;
        is_respawning = true;
        is_brightning = true;

        this.gameObject.SetActive(true);
    }

    public void Deactivate()
    {
        is_respawning = false;
        this.gameObject.SetActive(false);
    }

    public bool is_occupied()
    {
        return is_respawning;
    }
}
