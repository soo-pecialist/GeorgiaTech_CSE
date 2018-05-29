using UnityEngine;

public class PlayerShooting : MonoBehaviour
{
    public int damagePerShot = 20;
    public float timeBetweenBullets = 0.15f;
    public float range = 100f;


    float timer;
    Ray shootRay = new Ray();
    RaycastHit shootHit;
    int shootableMask;
    ParticleSystem gunParticles;
    LineRenderer gunLine;
    AudioSource gunAudio;
    Light gunLight;
    float effectsDisplayTime = 0.2f;

    void Awake ()
    {
        shootableMask = LayerMask.GetMask ("Shootable");
        gunParticles = GetComponent<ParticleSystem> ();
        gunLine = GetComponent <LineRenderer> ();
        gunAudio = GetComponent<AudioSource> ();
        gunLight = GetComponent<Light> ();
    }


    void Update ()
    {
        timer += Time.deltaTime;
		Color color = new Vector4(Random.value, Random.value, Random.value, 1.0f); // Random Color
	
		if(Input.GetButton ("Fire1") && timer >= timeBetweenBullets && Time.timeScale != 0)
        {
			// <change weapon effect color randomly every shoot>
//			gunLine.startColor = color;	// change LineRenderer startColor  
//			gunLine.endColor = color;	// change LineRenderer endColor 
			gunLine.material.color = color; // change LineRenderer color 
			gunLight.color = color;	// change Light color 
			gunLight.intensity = Random.Range (0.1f, 7.5f);	// for dramatic sensation, change Light intensity  
			gunParticles.startColor = color; // change particle system color

            Shoot ();
        }

        if(timer >= timeBetweenBullets * effectsDisplayTime)
        {
            DisableEffects ();
        }
    }


    public void DisableEffects ()
    {
        gunLine.enabled = false;
        gunLight.enabled = false;
    }


    void Shoot ()
    {
        timer = 0f;

        gunAudio.Play ();

        	gunLight.enabled = true;

        gunParticles.Stop (); //  if particle is still playing, stop them and play again from the get-go
        	gunParticles.Play ();

        gunLine.enabled = true;
        gunLine.SetPosition (0, transform.position); // 0 means first position of the line = end of barrel gun

        shootRay.origin = transform.position; // = gun barrel end. 
		shootRay.direction = transform.forward; // (0,0,1)

        if(Physics.Raycast (shootRay, out shootHit, range, shootableMask))
        {
            EnemyHealth enemyHealth = shootHit.collider.GetComponent <EnemyHealth> ();
            if(enemyHealth != null)
            {
                enemyHealth.TakeDamage (damagePerShot, shootHit.point);
            }
            gunLine.SetPosition (1, shootHit.point);
        }
        else
        {
            gunLine.SetPosition (1, shootRay.origin + shootRay.direction * range);
        }
    }
}
