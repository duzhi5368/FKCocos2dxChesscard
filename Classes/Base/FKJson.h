/********************************************************************
	created:	2016/07/19
	filename: 	FKJson
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include <string>
//------------------------------------------------------------
namespace FKJson
{
	/* Json Types: */
	#define Json_False 0
	#define Json_True 1
	#define Json_NULL 2
	#define Json_Number 3
	#define Json_String 4
	#define Json_Array 5
	#define Json_Object 6
	//------------------------------------------------------------
	/* The Json structure: */
	typedef struct Json 
	{
		struct Json* next;
		struct Json* prev; /* next/prev allow you to walk array/object chains. Alternatively, use getSize/getItemAt/getItem */
		struct Json* child; /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

		int type; /* The type of the item, as above. */

		const char* valuestring; /* The item's string, if type==Json_String */
		int valueint; /* The item's number, if type==Json_Number */
		float valuefloat; /* The item's number, if type==Json_Number */

		const char* name; /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
	} FKJson;
	//------------------------------------------------------------
	/* Supply a block of JSON, and this returns a Json object you can interrogate. Call Json_dispose when finished. */
	Json* Json_create (const char* value);

	/* Delete a Json entity and all subentities. */
	void Json_dispose (Json* json);

	/* Returns the number of items in an array (or object). */
	int Json_getSize (Json* json);

	/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
	Json* Json_getItemAt (Json* json, int item);

	/* Get item "string" from object. Case insensitive. */
	Json* Json_getItem (Json* json, const char* string);
	const char* Json_getString (Json* json, const char* name, const char* defaultValue);
	float Json_getFloat (Json* json, const char* name, float defaultValue);
	int Json_getInt (Json* json, const char* name, int defaultValue);

	/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when Json_create() returns 0. 0 when Json_create() succeeds. */
	const char* Json_getError (void);

	std::string	GetStrJson(Json* j);
	int			GetIntJson(Json* j);
	float		GetFloatJson(Json* j);
}