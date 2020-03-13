using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class PlayerController : MonoBehaviour 
{

	public float timeStamp;
	public string Shot1;
	public GameObject Bullet;
	public Transform SpawnDisparo;
	public float CooldownPeriodSeconds,RechargeTime;
	public int Ammunition;
	private int ControlMunition;

	void Start()
	{
		ControlMunition = Ammunition;
	}

	void Update()
		{
			if (Input.GetButton(Shot1) && Time.time > timeStamp) 
				{
				//Timer del cooldown, al usar la condicion se activa el cd
				timeStamp = Time.time + CooldownPeriodSeconds;
				Instantiate (Bullet, SpawnDisparo.position, SpawnDisparo.rotation);
				Ammunition--;
				}
			if (Ammunition == 0 || Input.GetKeyDown("r")) 
			{
				Debug.Log (Ammunition);
				timeStamp = Time.time + CooldownPeriodSeconds+RechargeTime;
				Ammunition = ControlMunition;
				Debug.Log (Ammunition);
			}
		}
}
