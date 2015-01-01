#ifndef NOCOPYASSIGN_H
#define NOCOPYASSIGN_H

//====================================================================================================
//	Filename:		NoCopyAssign.h
//	Created by:		Matt Stinnen
//	Discription:	
//====================================================================================================

//====================================================================================================
// Namespace
//====================================================================================================

namespace MAGE3D
{
namespace CORE
{
	class NoCopyAssign
	{
	public:
		NoCopyAssign(){}
		~NoCopyAssign(){}
	private:
		NoCopyAssign(const NoCopyAssign&);
		NoCopyAssign& operator=(const NoCopyAssign){}
	};
} // namespace CORE
}// namespace MAGE3D

#endif //NOCOPYASSIGN_H