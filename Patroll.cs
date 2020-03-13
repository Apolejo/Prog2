using UnityEngine;
using System.Collections;

public class Patroll : MonoBehaviour {

	private Animator Anim;
	public GameObject[] Points;
	//public Transform[] Points;
	private UnityEngine.AI.NavMeshAgent agent;
	private int destPoint = 0;
	public float tiempo;
	public int Control;
	// Use this for initialization
	void Start () 
	{
		//Anim = GetComponent<Animator> ();
		//Points = GameObject.FindGameObjectsWithTag ("Target");
		agent = GetComponent<UnityEngine.AI.NavMeshAgent> ();
		agent.autoBraking = false;
		GoNextSpot ();
	}

	// Update is called once per frame
	void GoNextSpot () 
	{
		if (Points.Length == 0) 
		{
			return;
		}

		agent.destination = Points[destPoint].transform.position;
		//Vector3 RdmSpot= new Vector3(Random.Range(-6,6),0,Random.Range(-6,6));
		//	agent.destination = RdmSpot;//Go for a random spot patroll, in this case he will go to the "reactor"
		//agent.destination = Points[destPoint].transform.position;//This lanes is for a patroll
		//destPoint = (destPoint + 1) % Points.Length;
	}

	void Update () 
	{
		if (agent.remainingDistance < 1f)
			DestroyImmediate(gameObject);
		/*tiempo += Time.deltaTime * 1;
		if (agent.remainingDistance < 1f)
			GoNextSpot();*/ //Those points are for patroll, we dont need patroll.
		/*if (agent.speed != 0) {
			Anim.SetBool ("Speed", true);
		} else 
		{
			Anim.SetBool ("Speed", false);
		}
		if (tiempo >= 15f) 
		{
			Control++;
			if (Control == 1) 
			{
				Anim.SetBool ("Death", true);
				agent.speed = 0f;
			}if (Control > 1) 
			{
				Anim.SetBool ("Death", false);
			}
		}*/
	}

}
