#include "stdafx.h"
#include "SceneManager.h"

void ClearDeletedObject(std::list<std::shared_ptr<Object>>& objectlist ,ObjectType objtype)
{
	////	삭제된 오브젝트 정리
	for (auto vectoriter = objectlist.begin(); vectoriter != objectlist.end();) {
		if (*vectoriter == nullptr)
			vectoriter = objectlist.erase(vectoriter);

		else if (vectoriter != objectlist.end())
			vectoriter++;
	}

}