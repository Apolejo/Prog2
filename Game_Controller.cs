using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Game_Controller : MonoBehaviour {

	// Use this for initialization
	public int Rounds,ManySpawns,SpawnWaitTime,RoundsWaitTime;
	public int GeneralRounds,GeneralWaitTime;
	//private Transform[] Spawns;
	private GameObject[] Spawns;
	public GameObject[] Enemys;

	[Tooltip("Cada cuanto Spawnea un add distinto")]

	public float Big_Add, Speedy_Add;

	[Tooltip("Cantidad que spawnean de cada uno")]
	public int Big_Add_Many, Speedy_Add_Many;

	public float First_Speed_Add,First_Big_Add;

	private float TIME;

	private int RndRounds=1;

	void Start () 
	{
		Spawns = GameObject.FindGameObjectsWithTag ("Spawn");
		StartCoroutine (CONTROL(GeneralRounds,RndRounds));
	}
	
	// Update is called once per frame
	void Update () 
	{
		TIME = Time.time;
		if (TIME >= 30f) 
		{
			StopAllCoroutines ();
	//		print ("SE ACABO PRIMERA FASE");
		}
	}

	IEnumerator CONTROL(int GR,int RR)
	{
		while (GR > 0) 
		{
			if (RR == 1) {
				StartCoroutine (EnemySpawn (ManySpawns));
			}
			if (RR == 2) {
				yield return new WaitForSeconds (Big_Add);
				StartCoroutine (EnemyBigSpawn (Big_Add, Big_Add_Many));			
			}
			if (RR == 3) {
				StartCoroutine (EnemySpeedSpawn (Speedy_Add, Speedy_Add_Many));
			}
			yield return new WaitForSeconds (GeneralWaitTime);
			GR--;
			RR = Random.Range(1,4);
	//		print ("RondaGeneral" + RR + GR);
		}
		StartCoroutine (CONTROL (GeneralRounds,RndRounds));
		StartCoroutine (CONTROL (GeneralRounds,RndRounds));
	}
	IEnumerator EnemySpawn (int SpawnsMany)
	{
		while (Rounds > 0) 
		{
			for (int i = 1; i <= SpawnsMany; i++) 
			{

				int SpawnsRandom = Random.Range (0, Spawns.Length);
				Instantiate (Enemys [1], Spawns [SpawnsRandom].transform.position, Quaternion.identity);
				yield return new WaitForSeconds (SpawnWaitTime);
			}
			SpawnsMany += 3;
			Rounds--;
		//	print ("NormalRonda" + Rounds);
			yield return new WaitForSeconds (RoundsWaitTime);
		}
	}
	IEnumerator EnemyBigSpawn (float BigAdd,int BigMany)
	{
		for (int i = 0; i <= BigMany; i++) 
			{
		//		print ("BigAdd");
				int SpawnsRandom = Random.Range (0, Spawns.Length);
				Instantiate (Enemys[2], Spawns[SpawnsRandom].transform.position , Quaternion.identity);
				yield return new WaitForSeconds (BigAdd);
			}

	}
	IEnumerator EnemySpeedSpawn (float SpeedAdd,int SpeedMany)
	{
		//	print ("SpeedAdd");
			int SpawnsRandom = Random.Range (0, Spawns.Length);
			for (int i = 0; i <= SpeedMany; i++) 
			{
				Instantiate (Enemys [0], Spawns [SpawnsRandom].transform.position, Quaternion.identity);
				yield return new WaitForSeconds (SpeedAdd);
			}
	}
}
