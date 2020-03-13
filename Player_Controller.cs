using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player_Controller : MonoBehaviour {

	// Use this for initialization

	public float MovSpeed,JumpSpeed;
	private Animator Anim;
	[Tooltip("Limites para X")]
	public float LimitX_Min,LimitX_Max;
	[Tooltip("Limites para Z")]
	public float LimitZ_Min,LimitZ_Max;
	void Start () 
	{
		Anim = GetComponent<Animator> ();
	}
	
	// Update is called once per frame
	void Update ()
	{
		float h = Input.GetAxis ("Horizontal");
		float v = Input.GetAxis ("Vertical");
		float j = Input.GetAxis ("Jump");
		Vector3 Move = new Vector3 (h, 0, v);
		Vector3 JumpForce = new Vector3 (0, j, 0);
		transform.Translate (Move * MovSpeed);
		GetComponent<Rigidbody> ().position = new Vector3 (
			Mathf.Clamp (GetComponent<Rigidbody> ().position.x, LimitX_Min, LimitX_Max),
			Mathf.Clamp (GetComponent<Rigidbody> ().position.y, 0, 2),
			Mathf.Clamp (GetComponent<Rigidbody> ().position.z, LimitZ_Min, LimitZ_Max));
		Anim.SetFloat ("VelocityX", h);
		Anim.SetFloat ("VelocityZ", v);
		Anim.SetFloat ("Speed", v);
		Anim.SetFloat ("Speed", h);
		if (Input.GetKeyDown ("space")) 
		{
			Anim.SetBool ("Jump_Anim", true);
			GetComponent<Rigidbody> ().AddForce (JumpForce*JumpSpeed);
		}
		if (Input.GetKeyUp ("space")) 
		{
			Anim.SetBool ("Jump_Anim", false);
		}
		if (Input.GetKeyDown ("left shift")) 
		{
			MovSpeed = MovSpeed * 2;
			Anim.SetBool ("Shift", true);
		}
		if (Input.GetKeyUp ("left shift")) 
		{
			MovSpeed = MovSpeed / 2;
			Anim.SetBool ("Shift", false);
		}
	}
}
