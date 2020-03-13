using UnityEngine;
using System.Collections;

public class DestroyByTime : MonoBehaviour {

	public float DestroyTime;
	void Start () 
	{
		Destroy (gameObject, DestroyTime);
	}

}
