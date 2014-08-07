//
//  RALAudioSource.h
//  rayne-openal
//
//  Copyright 2014 by Überpixel. All rights reserved.
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef __rayne_openal__RALAudioSource__
#define __rayne_openal__RALAudioSource__

#include <Rayne/Rayne.h>

namespace RN
{
	namespace openal
	{
		class AudioWorld;
		
		class AudioSource : public SceneNode
		{
		public:
			friend class AudioWorld;
			
			AudioSource(AudioResource *asset);
			~AudioSource() override;
			
			void Update(float delta) override;
			
			void Play();
			void SetRepeat(bool repeat);
			void SetPitch(float pitch);
			void SetGain(float gain);
			void SetRange(float min, float max);
			void SetSelfdestruct(bool selfdestruct);
			
		private:
			AudioWorld *_owner;
			AudioResource *_asset;
			
			uint32 _source;
			Vector3 _oldPosition;
			
			bool _isPlaying;
			bool _isRepeating;
			bool _isSelfdestructing;
			
			RNDeclareMeta(AudioSource)
		};
	}
}

#endif /* defined(__rayne_openal__RALAudioSource__) */
