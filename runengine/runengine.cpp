#include "pch.h"
#include "runengine.h"

// RANDiT **********************
dll::RANDiT::RANDiT()
{
	vSeeds.clear();
	for (int i = 0; i < 650; i++)vSeeds.push_back(rd());
	sq = new std::seed_seq(vSeeds.begin(), vSeeds.end());
	twister = new std::mt19937(*sq);
	vSeeds.clear();
}
dll::RANDiT::~RANDiT()
{
	delete sq;
	delete twister;
}
int dll::RANDiT::operator() (int min, int max)
{
	std::uniform_int_distribution do_engine(min, max);
	return do_engine(*twister);
}
///////////////////////////////

// PROTON *********************
dll::PROTON::PROTON(float _x, float _y, float _width, float _height)
{
	width = _width;
	height = _height;
	x = _x;
	y = _y;
	ex = x + width;
	ey = y + height;
}
void dll::PROTON::SetEdges()
{
	ex = x + width;
	ey = y + height;
}
void dll::PROTON::NewDims(float _new_width, float _new_height)
{
	width = _new_width;
	height = _new_height;
	ex = x + width;
	ey = y + height;
}
float dll::PROTON::GetWidth() const
{
	return width;
}
float dll::PROTON::GetHeight() const
{
	return height;
}
//////////////////////////////

// PROT_MESH*******************
dll::PROT_MESH::PROT_MESH(size_t array_size) :mArraySize{ array_size }, base_pointer{ new PROTON[array_size] }
{
	if (array_size > 0)validated = true;
}
dll::PROT_MESH::~PROT_MESH()
{
	delete[]base_pointer;
}
bool dll::PROT_MESH::is_vaild() const
{
	return validated;
}
size_t dll::PROT_MESH::size() const
{
	return mArraySize;
}
void dll::PROT_MESH::push_back(PROTON& element)
{
	if (current_position + 1 <= mArraySize)
	{
		*(base_pointer + current_position) = element;
		++current_position;
	}
}
void dll::PROT_MESH::push_front(PROTON& element)
{
	*(base_pointer) = element;
}
dll::PROTON& dll::PROT_MESH::operator[](size_t index)
{
	if (validated && index < mArraySize)return *(base_pointer + index);
	static PROTON ret{};
	return ret;
}
void dll::PROT_MESH::operator() (size_t index, PROTON& element)
{
	if (index >= mArraySize)return;
	*(base_pointer + index) = element;
}
//////////////////////////////

// BASE_CREATURE *************
dll::BASE_CREATURE::BASE_CREATURE(char what_type, float _sx, float _sy) :PROTON(_sx, _sy)
{
	type_flag = what_type;

	switch (type_flag)
	{
	case hero_type:
		NewDims(45.0f, 50.0f);
		strenght = 20;
		lifes = 100;
		speed = 1.0f;
		attack_delay = 100;
		max_frames = 3;
		frame_delay = 20;
		dir = dirs::right;
		break;

	case evil1_type:
		NewDims(40.0f, 40.0f);
		strenght = 5;
		lifes = 50;
		speed = 0.8f;
		attack_delay = 150;
		max_frames = 15;
		frame_delay = 4;
		break;

	case evil2_type:
		NewDims(40.0f, 40.0f);
		strenght = 8;
		lifes = 70;
		speed = 0.6f;
		attack_delay = 180;
		max_frames = 36;
		frame_delay = 2;
		break;

	case evil3_type:
		NewDims(45.0f, 45.0f);
		strenght = 8;
		lifes = 80;
		speed = 0.5f;
		attack_delay = 200;
		max_frames = 31;
		frame_delay = 2;
		break;

	case evil4_type:
		NewDims(40.0f, 30.0f);
		strenght = 3;
		lifes = 30;
		speed = 0.9f;
		attack_delay = 120;
		max_frames = 28;
		frame_delay = 3;
		break;

	case evil5_type:
		NewDims(40.0f, 45.0f);
		strenght = 6;
		lifes = 50;
		speed = 0.7f;
		attack_delay = 170;
		max_frames = 35;
		frame_delay = 2;
		break;
	}
}
void dll::BASE_CREATURE::SetPathInfo(float _dest_x, float _dest_y)
{
	move_sx = x;
	move_sy = y;
	move_ex = _dest_x;
	move_ey = _dest_y;

	hor_line = false;
	vert_line = false;

	if (abs(move_ex - move_sx) < width / 2)
	{
		vert_line = true;
		return;
	}
	if (abs(move_ey - move_sy) < height / 2)
	{
		hor_line = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sy);
	intercept = move_sy - move_sx * slope;
}
int dll::BASE_CREATURE::GetFrame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		switch (type_flag)
		{
		case hero_type:
			frame_delay = 20;
			break;

		case evil1_type:
			frame_delay = 4;
			break;

		case evil2_type:
			frame_delay = 2;
			break;

		case evil3_type:
			frame_delay = 2;
			break;

		case evil4_type:
			frame_delay = 3;
			break;

		case evil5_type:
			frame_delay = 2;
			break;
		}

		++frame;
		if (frame > max_frames)frame = 0;
	}
	return frame;
}
int dll::BASE_CREATURE::Attack()
{
	--attack_delay;
	if (attack_delay <= 0)
	{
		switch (type_flag)
		{
		case hero_type:
			attack_delay = 100;
			break;

		case evil1_type:
			attack_delay = 150;
			break;

		case evil2_type:
			attack_delay = 180;
			break;

		case evil3_type:
			attack_delay = 200;
			break;

		case evil4_type:
			attack_delay = 120;
			break;

		case evil5_type:
			attack_delay = 170;
			break;
		}

		return strenght;
	}

	return 0;
}
char dll::BASE_CREATURE::GetTypeFlag() const
{
	return type_flag;
}
char dll::BASE_CREATURE::GetMoveFlag() const
{
	return move_flag;
}
bool dll::BASE_CREATURE::CheckTypeFlag(char which_flag) const
{
	return type_flag & which_flag;
}
void dll::BASE_CREATURE::SetTypeFlag(char which_flag)
{
	type_flag |= which_flag;
}
void dll::BASE_CREATURE::NullTypeFlag(char which_flag)
{
	type_flag &= ~which_flag;
}
bool dll::BASE_CREATURE::CheckMoveFlag(char which_flag) const
{
	return move_flag & which_flag;
}
void dll::BASE_CREATURE::SetMoveFlag(char which_flag) 
{
	move_flag = 0;
	move_flag |= which_flag;
}
void dll::BASE_CREATURE::NullMoveFlag(char which_flag)
{
	move_flag &= ~which_flag;
}
////////////////////////////////

// HERO ************************
dll::HERO::HERO(float _where_x, float _where_y) :BASE_CREATURE(hero_type, _where_x, _where_y){}
float dll::HERO::GetXAxisMove(float gear) const
{
	if (dir == dirs::right)return -(speed + gear / 10);
	return speed + gear / 10;
}
char dll::HERO::Move(float gear, float dest_x, float dest_y, PROT_MESH& platforms)
{
	if (CheckMoveFlag(jump_up_flag) || CheckMoveFlag(jump_down_flag))return move_flag;

	move_flag = fall_flag;

	if (platforms.is_vaild())
	{
		for (int i = 0; i < platforms.size(); ++i)
		{
			if (!(x >= platforms[i].ex || ex <= platforms[i].x || y >= platforms[i].ey || ey <= platforms[i].y))
			{
				SetMoveFlag(run_flag);
				break;
			}
		}
	}

	return move_flag;
}
char dll::HERO::Jump(float gear, PROT_MESH& platforms)
{
	if (!CheckMoveFlag(jump_up_flag) && !CheckMoveFlag(jump_down_flag))
	{
		move_flag = 0;
		SetMoveFlag(jump_up_flag);
		move_sy = y;
		move_ey = y - 100.0f;
	}

	float my_speed = speed + gear / 10;

	if (CheckMoveFlag(jump_up_flag))
	{
		if (y > move_ey)
		{
			if (y - my_speed <= sky)
			{
				move_sy = y;
				move_ey = y + 100.0f;
				move_flag = jump_down_flag;
			}
			else
			{
				y -= my_speed;
				SetEdges();
			}
			return move_flag;
		}
		else
		{
			move_sy = y;
			move_ey = y + 100.0f;
			move_flag = jump_down_flag;
			return move_flag;
		}
	}
	else if (CheckMoveFlag(jump_down_flag))
	{
		if (ey + my_speed > ground)
		{
			y = ground - height;
			SetEdges();
			move_flag = stop_flag;
			return move_flag;
		}
		else
		{
			y += my_speed;
			SetEdges();
			if (platforms.is_vaild())
			{
				for (int i = 0; i < platforms.size(); i++)
				{
					if (!(x > platforms[i].ex || ex<platforms[i].x || y>platforms[i].ey || ey < platforms[i].y))
					{
						y = platforms[i].y - height;
						SetEdges();
						move_flag = stop_flag;
						break;
					}
				}
			}
		}
	}

	return move_flag;
}
char dll::HERO::Fall(float gear, PROT_MESH& platforms)
{
	float my_speed = speed + gear / 10;

	if (ey + my_speed > ground)
	{
		y = ground - height;
		SetEdges();
		move_flag = stop_flag;
		return move_flag;
	}

	y += my_speed;
	SetEdges();
	
	if (platforms.is_vaild())
	{
		for (int i = 0; i < platforms.size(); i++)
		{
			if (!(x > platforms[i].ex || ex<platforms[i].x || x>platforms[i].ex || ex < platforms[i].x))
			{
				y = platforms[i].y - height;
				SetEdges();
				move_flag = stop_flag;
				break;
			}
		}
	}
	return move_flag;
}
void dll::HERO::Release()
{
	delete this;
}
////////////////////////////////

// EVIL ************************
dll::EVIL::EVIL(char what, float _where_x, float _where_y) :BASE_CREATURE(hero_type, _where_x, _where_y) {}
float dll::EVIL::GetXAxisMove(float gear) const
{
	return 0;
}
char dll::EVIL::Move(float gear, float dest_x, float dest_y, PROT_MESH& platforms)
{
	move_flag = 0;

	float my_speed = speed + gear / 10.0f;

	if (platforms.is_vaild())
	{
		for (int i = 0; i < platforms.size(); ++i)
		{
			if (!(x >= platforms[i].ex || ex <= platforms[i].x || y >= platforms[i].ey || ey <= platforms[i].y))
			{
				SetMoveFlag(run_flag);
				break;
			}
		}
	}

	if (!move_flag)
	{
		SetMoveFlag(fall_flag);
		return move_flag;
	}

	if (x < dest_x)
	{
		if (ex + my_speed <= scr_width)
		{
			dir = dirs::right;
			x += my_speed;
			SetEdges();
		}
	}
	else if (x > dest_x)
	{
		if (x - my_speed >= 0)
		{
			dir = dirs::left;
			x -= my_speed;
			SetEdges();
		}
	}
	else SetMoveFlag(stop_flag);

	return move_flag;
}
char dll::EVIL::Jump(float gear, PROT_MESH& platforms)
{
	float my_speed = speed + gear / 10;

	if (!CheckMoveFlag(jump_up_flag) && !!CheckMoveFlag(jump_down_flag))
	{
		if (dir == dirs::left)SetPathInfo(x - width * 2, y - 100.0f);
		else if (dir == dirs::right)SetPathInfo(x + width * 2, y - 100.0f);
		move_flag = jump_up_flag;
		return move_flag;
	}
	else
	{
		if (CheckMoveFlag(jump_up_flag))
		{
			if (vert_line)
			{
				if (y - my_speed < sky || y < move_ey)
				{
					move_flag = jump_down_flag;
					return move_flag;
				}
				y -= my_speed;
				SetEdges();
				return move_flag;
			}
			if (move_sx > move_ex)
			{
				x -= my_speed;
				y = x * slope + intercept;
				SetEdges();
				if (x <= move_ex)vert_line = true;
				return move_flag;
			}
			else if (move_sx < move_ex)
			{
				x += my_speed;
				y = x * slope + intercept;
				SetEdges();
				if (ex > move_ex)vert_line = true;
				return move_flag;
			}
		}
		else if (CheckMoveFlag(jump_down_flag))
		{
			if (vert_line)
			{
				if (ey + my_speed > ground)
				{
					move_flag = run_flag;
					y = ground - height;
					SetEdges();
					return move_flag;
				}
				else if (ey > move_ey)
				{
					if (platforms.is_vaild())
					{
						for (int i = 0; i < platforms.size(); ++i)
						{
							if (!(x > platforms[i].x || ex<platforms[i].ex || y>platforms[i].y || ey < platforms[i].ey))
							{
								y = platforms[i].y - height;
								SetEdges();
								move_flag = run_flag;
								return move_flag;
								break;
							}
						}

					}
					move_flag = fall_flag;
					return move_flag;
				}
				else
				{
					y += my_speed;
					SetEdges();
					return move_flag;
				}
			}
			if (move_sx > move_ex)
			{
				x -= my_speed;
				y = x * slope + intercept;
				SetEdges();
				if (x <= move_ex)vert_line = true;
				if (ey > ground)
				{
					move_flag = run_flag;
					y = ground - height;
					SetEdges();
					return move_flag;
				}
				return move_flag;
			}
			else if (move_sx < move_ex)
			{
				x += my_speed;
				y = x * slope + intercept;
				SetEdges();
				if (ex > move_ex)vert_line = true;
				if (ey > ground)
				{
					move_flag = run_flag;
					y = ground - height;
					SetEdges();
					return move_flag;
				}
				return move_flag;
			}
		}
	}
	return move_flag;
}
char dll::EVIL::Fall(float gear, PROT_MESH& platforms)
{
	float my_speed = speed + gear / 10;

	if (ey + my_speed > ground)
	{
		y = ground - height;
		SetEdges();
		move_flag = stop_flag;
		return move_flag;
	}

	y += my_speed;
	SetEdges();

	if (platforms.is_vaild())
	{
		for (int i = 0; i < platforms.size(); i++)
		{
			if (!(x > platforms[i].ex || ex<platforms[i].x || x>platforms[i].ex || ex < platforms[i].x))
			{
				y = platforms[i].y - height;
				SetEdges();
				move_flag = run_flag;
				break;
			}
		}
	}
	return move_flag;
}
void dll::EVIL::Release()
{
	delete this;
}
//////////////////////////////////

// FACTORY ***********************

dll::Creature dll::CreatureFactory(char what_type, float start_x, float start_y)
{
	Creature ret = nullptr;

	switch (what_type)
	{
	case hero_type:
		ret = new HERO(start_x, start_y);
		break;

	case evil1_type:
		ret = new EVIL(evil1_type, start_x, start_y);
		break;

	case evil2_type:
		ret = new EVIL(evil2_type, start_x, start_y);
		break;

	case evil3_type:
		ret = new EVIL(evil3_type, start_x, start_y);
		break;

	case evil4_type:
		ret = new EVIL(evil4_type, start_x, start_y);
		break;

	case evil5_type:
		ret = new EVIL(evil5_type, start_x, start_y);
		break;
	}
	return ret;
}