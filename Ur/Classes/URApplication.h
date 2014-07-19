//
//  URApplication.h
//  Ur
//
//  Created by Sidney Just on 19/07/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __UR_APPLICATION_H__
#define __UR_APPLICATION_H__

#include <Rayne/Rayne.h>
#include "URWorld.h"

namespace UR
{
	class Application : public RN::Application
	{
	public:
		Application();
		~Application() override;
		
		void Start() override;
		void WillExit() override;
		
		void StartClient();
		void StartServer();
	};
}

#endif /* __UR_APPLICATION_H__ */
