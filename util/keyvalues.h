#ifndef KEYVALUES_H
#define KEYVALUES_H

#include <QtGlobal>
#include <string>
#include "util.h"



/// Implements implicit conversion between any data types
/// and caches results for faster access
class VarData
{
	enum
	{
		DATATYPE_STRING = (1 << 0),
		DATATYPE_INTEGER = (1 << 1),
        DATATYPE_BOOL = (1 << 2),
        DATATYPE_FLOAT = (1 << 3),
	};

public:
	VarData()
		: dataString(nullptr)
		, dataInteger(0)
        , dataBool(false)
        , dataFloat(0.0f)
		, cachedTypes(DATATYPE_INTEGER)
		, currentType(DATATYPE_INTEGER)
	{
	}

	~VarData()
	{
		delete [] dataString;
	}

	inline const char *GetString()
	{
		if ((cachedTypes & DATATYPE_STRING) == 0)
		{
			cachedTypes |= DATATYPE_STRING;

            char tempString[32] = { 0 };
			delete [] dataString;

			switch (currentType)
			{
			case DATATYPE_INTEGER:
                _snprintf_s(tempString, sizeof(tempString), sizeof(tempString), "%i", dataInteger);
				break;
            case DATATYPE_BOOL:
                _snprintf_s(tempString, sizeof(tempString), sizeof(tempString), "%i", (dataBool ? 1 : 0));
                break;
            case DATATYPE_FLOAT:
                _snprintf_s(tempString, sizeof(tempString), sizeof(tempString), "%f", dataFloat);
                break;
			default:
                Q_ASSERT(0);
			}

            dataString = _strcreatecpy(tempString);
		}

		return (dataString != nullptr) ? dataString : "";
	}

	inline void SetString(const char *value)
	{
		ChangeType(DATATYPE_STRING);

		delete [] dataString;
        dataString = _strcreatecpy(value);
	}

	inline int GetInt()
	{
		if ((cachedTypes & DATATYPE_INTEGER) == 0)
		{
			cachedTypes |= DATATYPE_INTEGER;

			switch (currentType)
			{
			case DATATYPE_STRING:
				dataInteger = atoi(dataString);
				break;
            case DATATYPE_BOOL:
                dataInteger = dataBool ? 1 : 0;
                break;
            case DATATYPE_FLOAT:
                dataInteger = int(dataFloat);
                break;
			default:
                Q_ASSERT(0);
			}
		}

		return dataInteger;
	}

	inline void SetInt(int value)
	{
		ChangeType(DATATYPE_INTEGER);

		dataInteger = value;
	}

    inline bool GetBool()
    {
        if ((cachedTypes & DATATYPE_BOOL) == 0)
        {
            cachedTypes |= DATATYPE_BOOL;

            switch (currentType)
            {
            case DATATYPE_STRING:
                dataBool = _streq("true", dataString)
                        || atoi(dataString) != 0;
                break;
            case DATATYPE_INTEGER:
                dataBool = (dataInteger != 0);
                break;
            case DATATYPE_FLOAT:
                dataBool = dataFloat != 0.0f;
                break;
            default:
                Q_ASSERT(0);
            }
        }

        return dataBool;
    }

    inline void SetBool(bool value)
    {
        ChangeType(DATATYPE_BOOL);

        dataBool = value;
    }

    inline float GetFloat()
    {
        if ((cachedTypes & DATATYPE_FLOAT) == 0)
        {
            cachedTypes |= DATATYPE_FLOAT;

            switch (currentType)
            {
            case DATATYPE_STRING:
                dataFloat = atof(dataString);
                break;
            case DATATYPE_INTEGER:
                dataFloat = float(dataInteger);
                break;
            case DATATYPE_BOOL:
                dataFloat = dataBool ? 1.0f : 0.0f;
                break;
            default:
                Q_ASSERT(0);
            }
        }

        return dataFloat;
    }

    inline void SetFloat(float value)
    {
        ChangeType(DATATYPE_FLOAT);

        dataFloat = value;
    }

private:
	void ChangeType(int type)
	{
		currentType = type;
		cachedTypes = type;
	}

	int cachedTypes;
	int currentType;

	char *dataString;
	int dataInteger;
    bool dataBool;
    float dataFloat;
};

/// Used to store a variable amount of data of any data type
class KeyValues
{
public:
	KeyValues()
		: name(nullptr)
		, peer(nullptr)
		, child(nullptr)
	{
	}

	KeyValues(const char *name)
		: name(nullptr)
		, peer(nullptr)
		, child(nullptr)
	{
		SetName(name);
	}

	inline void Release()
	{
		if (child != nullptr)
		{
			child->Release();
		}

		if (peer != nullptr)
		{
			peer->Release();
		}

		delete this;
	}

	inline const char *GetName() const
	{
		return (name != nullptr) ? name : "";
	}

	inline void SetName(const char *name)
	{
		delete [] this->name;

        this->name = _strcreatecpy(name);
	}

	inline const char *GetString(const char *name = nullptr, const char *defaultValue = "")
	{
		KeyValues *keyValues = FindChild(name);

		if (keyValues != nullptr)
		{
			return keyValues->value.GetString();
		}
		else
		{
			return defaultValue;
		}
	}

	inline void SetString(const char *name, const char *value)
	{
		FindOrCreateChild(name)->value.SetString(value);
	}

	inline int GetInt(const char *name = nullptr, int defaultValue = 0)
	{
		KeyValues *keyValues = FindChild(name);

		if (keyValues != nullptr)
		{
			return keyValues->value.GetInt();
		}
		else
		{
			return defaultValue;
		}
	}

	inline void SetInt(const char *name, int value)
	{
		FindOrCreateChild(name)->value.SetInt(value);
	}

    inline int GetBool(const char *name = nullptr, bool defaultValue = false)
    {
        KeyValues *keyValues = FindChild(name);

        if (keyValues != nullptr)
        {
            return keyValues->value.GetBool();
        }
        else
        {
            return defaultValue;
        }
    }

    inline void SetBool(const char *name, bool value)
    {
        FindOrCreateChild(name)->value.SetBool(value);
    }

    inline float GetFloat(const char *name = nullptr, float defaultValue = 0.0f)
    {
        KeyValues *keyValues = FindChild(name);

        if (keyValues != nullptr)
        {
            return keyValues->value.GetFloat();
        }
        else
        {
            return defaultValue;
        }
    }

    inline void SetFloat(const char *name, float value)
    {
        FindOrCreateChild(name)->value.SetFloat(value);
    }

private:
	~KeyValues()
	{
		delete [] name;
	}

	KeyValues *FindChild(const char *name)
	{
		if (name == nullptr)
		{
			return this;
		}

		for (KeyValues *search = child;
			search != nullptr;
			search = search->peer)
		{
            if (_stricmp(name, search->GetName()) == 0)
			{
				return search;
			}
		}

		return nullptr;
	}

	KeyValues *FindOrCreateChild(const char *name)
	{
		KeyValues *search = FindChild(name);

		if (search == nullptr)
		{
			search = new KeyValues();
			search->SetName(name);

			search->peer = child;
			child = search;
		}

		return search;
	}

	KeyValues *peer;
	KeyValues *child;

	char *name;
	VarData value;

};


#endif
