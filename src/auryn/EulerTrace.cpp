/* 
* Copyright 2014-2016 Friedemann Zenke
*
* This file is part of Auryn, a simulation package for plastic
* spiking neural networks.
* 
* Auryn is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Auryn is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Auryn.  If not, see <http://www.gnu.org/licenses/>.
*
* If you are using Auryn or parts of it for your work please cite:
* Zenke, F. and Gerstner, W., 2014. Limits to high-speed simulations 
* of spiking neural networks using general-purpose computers. 
* Front Neuroinform 8, 76. doi: 10.3389/fninf.2014.00076
*/

#include "EulerTrace.h"

using namespace auryn;

void EulerTrace::init(NeuronID n, AurynFloat timeconstant) 
{
	// size = n;
	// set_timeconstant(timeconstant);
	// state = new AurynStateVector ( calculate_vector_size(size) ); 
	temp = new AurynStateVector ( calculate_vector_size(size) ); // temp vector
	set_all(0.);
	target_ptr = NULL;
}

void EulerTrace::free()
{
	delete temp;
}

EulerTrace::EulerTrace(NeuronID n, AurynFloat timeconstant) : Trace(n, timeconstant)
{
	init(n,timeconstant);
}

EulerTrace::~EulerTrace()
{
	free();
}

void EulerTrace::set_timeconstant(AurynFloat timeconstant)
{
	super::set_timeconstant(timeconstant);
	mul_follow = auryn_timestep/tau;
	scale_const = std::exp(-mul_follow);
}

void EulerTrace::set_target( AurynStateVector * target )
{
	if ( target != NULL ) {
		target_ptr = target ;
		copy(target);
	}
}

void EulerTrace::evolve()
{
	scale(scale_const);
}

void EulerTrace::follow()
{ 
	temp->diff(this,target_ptr);
	saxpy(-auryn_timestep/tau, temp );
}

