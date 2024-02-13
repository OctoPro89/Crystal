using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class GameManager : Entity
{
	private AudioComponent m_AudioComponent;

	void StopMusic()
	{
		m_AudioComponent.Stop(); 
	}

	void RestartMusic()
	{
		StopMusic();
		m_AudioComponent.Play();
	}

	void OnCreate()
	{
		m_AudioComponent = GetComponent<AudioComponent>();
		m_AudioComponent.Play(); /* Start the music */
		CrystalUtils.FuncAfterDelay(109, RestartMusic); /* Restart the music after it's done, 109 seconds is the duration */
	}
}
