/////////////////////////////////////////////////////////////////////////////////////////
// room.h
#ifndef _ROOMS_H_
#define _ROOMS_H_

//======================================================================================

#include <winsock2.h>

//======================================================================================

struct User
{
	char Name[32];
	SOCKET Socket;
	sockaddr_in Addres;
};

//--------------------------------------------------------------------------------------

struct Room
{
	int Count;
	char Name[32];
	User *List;

	void Add(User user);
	void Del(char *name);
	Room operator= (const Room r);
};

//=======================================================================================

void Room::Add(User user)
{
	User *tmp = new User[Count + 1];

	for(int i = 0; i < Count; i++)
		tmp[i] = List[i];
	tmp[Count].Addres = user.Addres;
	tmp[Count].Socket = user.Socket;
	lstrcpyn(tmp[Count++].Name, user.Name, 31);

	if((Count - 1) != 0)
		delete List;
	List = tmp;
}

//---------------------------------------------------------------------------------------

void Room::Del(char *name)
{
	User *tmp = new User[Count - 1];
	int i = 0, j = 0, n = 0;
	
	for(i = 0; i < Count; i++)
	{
		if(!lstrcmp(name, List[i].Name))
			break;
	}
	
	n = i;
	
	for(i = 0; i < n; i++)
	{
		tmp[i].Addres = List[i].Addres;
		tmp[i].Socket = List[i].Socket;
		lstrcpy(tmp[i].Name, List[i].Name);
	}

	for(j = n, i = n + 1; i < Count - n; i++, j++)
	{
		tmp[j].Addres = List[i].Addres;
		tmp[j].Socket = List[i].Socket;
		lstrcpy(tmp[j].Name, List[i].Name);
	}

	Count--;
	delete List;
	List = tmp;
}

//---------------------------------------------------------------------------------------

Room Room::operator= (const Room r)
{
	Count = r.Count;
	lstrcpy(Name, r.Name);
	List = new User[Count];
	for(int i = 0; i < Count; i++)
		List[i] = r.List[i];
	return *this;
}

//=======================================================================================

#endif

/////////////////////////////////////////////////////////////////////////////////////////