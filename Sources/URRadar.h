//
//  URRadar.h
//  Ur
//
//  Created by Nils Daumann on 20.07.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __UR_RADAR_H__
#define __UR_RADAR_H__

#include <Rayne.h>

namespace UR
{
	class Radar : public RN::Entity
	{
	public:
		Radar();
		~Radar();
		void Update(float delta) override;
		
	protected:
		void DidUpdate(ChangeSet changeSet) override;
		
	private:
		std::vector<RN::Entity *> _ships;
		RN::Model *_shipModel;
	};
}

#endif
