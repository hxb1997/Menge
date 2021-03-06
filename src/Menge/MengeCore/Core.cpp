/*

License

Menge
Copyright ?and trademark ?2012-14 University of North Carolina at Chapel Hill. 
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation 
for educational, research, and non-profit purposes, without fee, and without a 
written agreement is hereby granted, provided that the above copyright notice, 
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North 
Carolina at Chapel Hill. The software program and documentation are supplied "as is," 
without any accompanying services from the University of North Carolina at Chapel 
Hill or the authors. The University of North Carolina at Chapel Hill and the 
authors do not warrant that the operation of the program will be uninterrupted 
or error-free. The end-user understands that the program was developed for research 
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS 
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE 
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY 
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY 
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS 
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#include "MengeCore/Core.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQuery.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/Agents/Events/EventSystem.h"
#include "MengeCore/MatrixMy.h"



namespace Menge {
	BFSM::FSM * ACTIVE_FSM = 0x0;

	float SIM_TIME = 0.f;

	float SIM_TIME_STEP = 0.f;

	Agents::SpatialQuery * SPATIAL_QUERY = 0x0;

	Agents::Elevation * ELEVATION = 0x0;

	Agents::SimulatorInterface * SIMULATOR = 0x0;

	Menge::EventSystem * EVENT_SYSTEM = new Menge::EventSystem();

	size_t PROJECTNAME = 0;

	namespace Business {
		MatrixDim2* ProbStatesNormal = new MatrixDim2(3, 3, 1);//??????????????????????????????state??????????????
		MatrixDim2* ProbStatesCoupon = new MatrixDim2(3, 3, 1);
		MatrixDim3* ProbGoalsNormal = new MatrixDim3(3, 3, 10, 1);//??????????????????????????????state??????????????????id0~9????????????????????
		MatrixDim3* ProbGoalsCoupon = new MatrixDim3(3, 3, 10, 1);
	}

	namespace BusinessReality {
		MatrixDim4* ProbGoals = new MatrixDim4(3, 30, 3, 10, 1);
		//????????????????????????????????30??????????????????????????????????????id????????????????????
	}

}
namespace MengeVis {

	Viewer::GLViewer* SimViewer = 0x0;

}
