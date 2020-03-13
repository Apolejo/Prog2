using UnityEngine;
using System.Collections;

public class DestroyBullet : MonoBehaviour {

	void OnTriggerEnter (Collider other) 
	{
		if (other.tag == "Enemy")
		{
			Destroy (gameObject);
			Destroy (other.gameObject);
		}
		if (other.tag == "Obstacle")
		{
			Destroy (gameObject);
		}
	}
}