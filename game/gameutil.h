#ifndef GAMEUTIL_H
#define GAMEUTIL_H

#include <string>
#include <cstring>

#include "platform.h"

#define RESOURCE_OBSTACLE_MATERIAL_COUNT 3
#define RESOURCE_OBSTACLE_SOUND_COUNT 5
#define RESOURCE_FIGHTER_SOUND_COUNT 5

class Material;

struct Resource_Projectile_t
{
    Resource_Projectile_t()
    {
        fire_rate = 1;
        speed = 100;
        size = 5;
    }

    std::string name;

    std::string particle_trail;
    std::string particle_spawn;
    std::string particle_hit;

    std::string sound;

    float fire_rate;
    float speed;
    float size;
};

struct Resource_Destructible_t
{
    Resource_Destructible_t()
    {
        health = 1;
        score = 5;
        sound_hit_count = 0;
        sound_destroy_count = 0;
    }

    std::string name;

    int health;
    int score;

    std::string particle_hit;
    std::string particle_destroy;

    int sound_hit_count;
    int sound_destroy_count;
    std::string sound_hit[RESOURCE_FIGHTER_SOUND_COUNT];
    std::string sound_destroy[RESOURCE_FIGHTER_SOUND_COUNT];
};

struct Resource_Fighter_t : public Resource_Destructible_t
{
    Resource_Fighter_t()
    {
        material = nullptr;
    }

    Material *material;

    Resource_Projectile_t projectile;

    int size;

    std::string ai;
};

struct Resource_Obstacle_t : public Resource_Destructible_t
{
    Resource_Obstacle_t()
    {
        memset(materials, 0, sizeof(materials));
        materialCount = 0;

        health_min = 1;
        health_max = 1;
        size_min = 20;
        size_max = 20;
        angularvelocity_min = 50;
        angularvelocity_max = 100;
        velocity_x_min = 50;
        velocity_x_min = 150;
        origin_x_offset = 0;
    }

    int materialCount;
    Material *materials[RESOURCE_OBSTACLE_MATERIAL_COUNT];

    int health_min;
    int health_max;

    float size_min;
    float size_max;

    float angularvelocity_min;
    float angularvelocity_max;

    float velocity_x_min;
    float velocity_x_max;

    float origin_x_offset;
};


#endif // GAMEUTIL_H
