//
//  RALAudioListener.h
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

#ifndef __rayne_openal__RALAudioListener__
#define __rayne_openal__RALAudioListener__

#include <Rayne/Rayne.h>

namespace RN
{
	namespace openal
	{
		class AudioWorld;
		
		class AudioListener : public SceneNodeAttachment
		{
		public:
			friend class AudioWorld;
			
			AudioListener();
			~AudioListener() override;
			
			void Update(float delta);
			
		protected:
			void DidUpdate(SceneNode::ChangeSet changeSet) override;
			void DidAddToParent() override;
			void WillRemoveFromParent() override;
			
			void ReInsertIntoWorld();
			virtual void InsertIntoWorld(AudioWorld *world);
			virtual void RemoveFromWorld(AudioWorld *world);
			
		private:
			AudioWorld *_owner;
			
			Vector3 _oldPosition;
			
			RNDeclareMeta(AudioListener)
		};
	}
}

#endif /* defined(__rayne_openal__RALAudioListener__) */
