using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//Team: A-Team
//Team Members: David Burke, Kara Stolze, Shuangke Li, Marvin Cangcianno, Soo Hyeon Kim
using UnityEngine.UI;

public class ScrollCredits : MonoBehaviour {

    public Text[] creditsText;

    public float scrollSpeed = 0.2f;

    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        foreach (Text text in creditsText)
        {
            text.transform.Translate(Vector3.up * scrollSpeed);
        }
    }
}
