using UnityEngine;
using System.Collections;

public class Gizmo : MonoBehaviour {

	public float radio;
	// Use this for initialization
	void OnDrawGizmos () 
	{
		Gizmos.color = Color.black;
		Gizmos.DrawSphere (transform.position, radio);
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
