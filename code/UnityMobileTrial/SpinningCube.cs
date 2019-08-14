using UnityEngine;
using System.Collections;
using UnityEngine.Networking;

// [System.Serializable]
// public class DataClass {
//     public int 		myInt;
//     public float 	myFloat;
// }

public class SpinningCube : MonoBehaviour 
{
	public float m_Speed = 20f;
	
    //public DataClass[] myClass;

	private Vector3 m_RotationDirection = Vector3.up;

	public void ToggleRotationDirection()
	{
		Debug.Log ("Toggling rotation direction");

		//myClass.myFloat = Vector3.up;
		///GetRequest("http://192.168.1.1/");
		StartCoroutine(GetRequest("http://192.168.1.1/getEncoder"));

		if (m_RotationDirection == Vector3.up) 
		{
			m_RotationDirection = Vector3.down;       
			// Widen the object by 0.1
        	transform.localScale += new Vector3(0.1F, 0, 0);
						
			// Sphere? is 1 unit in size.  Drop floor by half this.
			///transform.position += new Vector3(0.0f, -0.5f, 0.0f);
		}
		else 
		{
			m_RotationDirection = Vector3.up;      
			 // Widen the object by 0.1
        	transform.localScale += new Vector3(0.1F, 0, 0);
		}
	}

	void Update() 
	{
		transform.Rotate(m_RotationDirection * Time.deltaTime * m_Speed);
	}


	// according https://docs.unity3d.com/ScriptReference/Networking.UnityWebRequest.Get.html
	//           https://docs.unity3d.com/Manual/UnityWebRequest-RetrievingTextBinaryData.html
	// 			 https://stackoverflow.com/questions/46003824/sending-http-requests-in-c-sharp-with-unity/46008025
	void Start()
	{
		StartCoroutine(GetRequest("http://192.168.1.1/getEncoder"));
	}
	
    IEnumerator GetRequest(string uri)
    {
		Debug.Log ("inside web request");

        using (UnityWebRequest webRequest = UnityWebRequest.Get(uri))
        {
			Debug.Log ("inside web request 2 ");
            // Request and wait for the desired page.
            yield return webRequest.SendWebRequest();

            string[] 	pages 	= uri.Split('/');
            int 		page 	= pages.Length - 1;

            if (webRequest.isNetworkError)
            {
                Debug.Log(pages[page] + ": Error: " + webRequest.error);
            }
            else
            {
                Debug.Log(pages[page] + ":\nReceived: " + webRequest.downloadHandler.text);
            }
        }
    }

}
