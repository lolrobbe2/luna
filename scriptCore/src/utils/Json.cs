﻿using System;
using System.Collections.Generic;

namespace Luna
{


    class Json
    {
        private readonly Dictionary<string, object> data;

        public Json()
        {
            data = new Dictionary<string, object>();
        }

        public void Add(string key, string value)
        {
            data[key] = value;
        }

        public void Add(string key, int value)
        {
            data[key] = value;
        }

        public void Add(string key, Json value)
        {
            data[key] = value;
        }

        public override string ToString()
        {
            return ToJsonString();
        }

        private string ToJsonString()
        {
            string jsonString = "{";

            foreach (var entry in data)
            {
                jsonString += $"\"{entry.Key}\": ";

                if (entry.Value is string)
                {
                    jsonString += $"\"{entry.Value}\", ";
                }
                else if (entry.Value is int)
                {
                    jsonString += $"{entry.Value}, ";
                }
                else if (entry.Value is Json)
                {
                    jsonString += ((Json)entry.Value).ToJsonString() + ", ";
                }
            }

            if (jsonString.EndsWith(", "))
            {
                jsonString = jsonString.Remove(jsonString.Length - 2);
            }

            jsonString += "}";

            return jsonString;
        }
        public static Json Parse(string json)
        {
            Json result = new Json();

            if (json.StartsWith("{") && json.EndsWith("}"))
            {
                json = json.Substring(1, json.Length - 2);
                string[] pairs = json.Split(',');

                foreach (var pair in pairs)
                {
                    string[] keyValue = pair.Split(':');
                    if (keyValue.Length == 2)
                    {
                        string key = keyValue[0].Trim().Trim('"');
                        string value = keyValue[1].Trim();

                        if (value.StartsWith("\"") && value.EndsWith("\""))
                        {
                            result.Add(key, value.Trim('"'));
                        }
                        else if (int.TryParse(value, out int intValue))
                        {
                            result.Add(key, intValue);
                        }
                        else if (value.StartsWith("{") && value.EndsWith("}"))
                        {
                            result.Add(key, Parse(value));
                        }
                        else
                        {
                            Log.Error("Error: Invalid JSON format for value of key {0}",key);
                        }
                    }
                    else
                    {
                        Log.Error("Error: Invalid JSON format");
                    }
                }
            }
            else
            {
                Log.Error("Error: Invalid JSON format");
            }

            return result;
        }
        public object this[string key]
        {
            get
            {
                if (data.ContainsKey(key))
                {
                    return data[key];
                }
                else
                {
                    Log.Error("Error: Key {0} not found in JSON object",key);
                    return null; // Or throw an exception if you prefer
                }
            }
        }
    }

}
