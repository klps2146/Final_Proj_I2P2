cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_TowerDefense)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2025_I2P2_TowerDefense
    Bullet/Bullet.cpp
    Bullet/Bullet.hpp
    Bullet/CoolBullet.cpp
    Bullet/CoolBullet.hpp
    Bullet/FireBullet.cpp
    Bullet/FireBullet.hpp
    Bullet/LaserBullet.cpp
    Bullet/LaserBullet.hpp
    Bullet/OrbitingBullet.cpp
    Bullet/OrbitingBullet.hpp



    Bullet/WeaponBullet.cpp
    Bullet/WeaponBullet.hpp
    Bullet/FireWeaponBullet.cpp
    Bullet/FireWeaponBullet.hpp


    # OK TODO HACKATHON-3 (3/3): Add the new enemy source files here
    Enemy/PlaneEnemy.cpp
    Enemy/PlaneEnemy.hpp

    Enemy/Enemy.cpp
    Enemy/Enemy.hpp
    Enemy/GodEnemy.cpp
    Enemy/GodEnemy.hpp
    Enemy/SoldierEnemy.cpp
    Enemy/SoldierEnemy.hpp
    Enemy/TankEnemy.cpp
    Enemy/TankEnemy.hpp
    Enemy/EnemyBullet.cpp
    Enemy/EnemyBullet.hpp
    Enemy/EnemyBullet1.cpp
    Enemy/EnemyBullet1.hpp

    Enemy/EnemyBullet2.cpp
    Enemy/EnemyBullet2.hpp


    Enemy/ShootEnemy.cpp
    Enemy/ShootEnemy.hpp
    Enemy/Bomb.cpp
    Enemy/Bomb.hpp  


    Enemy/SimpleBomb.cpp
    Enemy/SimpleBomb.hpp  

    Enemy/BombThrowerEnemy.cpp
    Enemy/BombThrowerEnemy.hpp


    Enemy/BossEnemy.cpp
    Enemy/BossEnemy.hpp
    Enemy/Boss1.cpp
    Enemy/Boss1.hpp
    Enemy/Boss2.cpp
    Enemy/Boss2.hpp
    Enemy/Boss3.cpp
    Enemy/Boss3.hpp






    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp
    Engine/SpriteFixed.cpp
    Engine/SpriteFixed.hpp 
    
    # OK TODO HACKATHON-2 (3/3): Add the new scene source files here
    Scene/SettingsScene.cpp
    Scene/SettingsScene.hpp
    Scene/ScoreboardScene.cpp
    Scene/ScoreboardScene.hpp

    Character/Character.cpp
    Character/Character.hpp
    
    Scene/LoseScene.cpp
    Scene/LoseScene.hpp
    Scene/PlayScene.cpp
    Scene/PlayScene.hpp
    Scene/StageSelectScene.cpp
    Scene/StageSelectScene.hpp
    Scene/StartScene.cpp
    Scene/StartScene.h
    Scene/WinScene.cpp
    Scene/WinScene.hpp
    Scene/ChatScene.cpp
    Scene/ChatScene.hpp

    Tool/ShovelTool.cpp
    Tool/ShovelTool.hpp
    
    Turret/CoolTurret.cpp
    Turret/CoolTurret.hpp
    Turret/LaserTurret.cpp
    Turret/LaserTurret.hpp
    Turret/MachineGunTurret.cpp
    Turret/MachineGunTurret.hpp
    Turret/Turret.cpp
    Turret/Turret.hpp
    Turret/TurretButton.cpp
    Turret/TurretButton.hpp

    UI/Animation/DirtyEffect.cpp
    UI/Animation/DirtyEffect.hpp
    UI/Animation/ExplosionEffect.cpp
    UI/Animation/ExplosionEffect.hpp
    UI/Animation/Plane.cpp
    UI/Animation/Plane.hpp

    UI/Component/ColoredRectangle.cpp
    UI/Component/ColoredRectangle.hpp
    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp
    UI/Effect/ParticleSystemManager.cpp
    UI/Effect/ParticleSystemManager.hpp
    UI/Effect/Particle.cpp
    UI/Effect/Particle.hpp

    Weapon/BulletBoom.cpp
    Weapon/BulletBoom.hpp
    Weapon/Gun.cpp
    Weapon/Gun.hpp

    Weapon/Weapon.cpp
    Weapon/Weapon.hpp

    Weapon/MeleeWeapon.cpp
    Weapon/MeleeWeapon.hpp
    Weapon/WeaponManager.hpp
    Weapon/WeaponManager.cpp

    ItemBar/Itembar.cpp
    ItemBar/Itembar.hpp
    Store/Store.cpp
    Store/Store.hpp

    Skill/SkillBase.cpp
    Skill/SkillBase.hpp
    Skill/DashSkill.cpp
    Skill/DashSkill.hpp
    Skill/AreaSkill.hpp
    Skill/AreaSkill.cpp
    Skill/SummonDroneSkill.cpp
    Skill/SummonDroneSkill.hpp
    Skill/ShieldSkill.cpp
    Skill/ShieldSkill.hpp
    Skill/NuclearSkill.cpp
    Skill/NuclearSkill.hpp
    Skill/OrbitBulletSkill.cpp
    Skill/OrbitBulletSkill.hpp

    Drop/coin.hpp
    Drop/coin.cpp

    Minimap/Minimap.cpp
    Minimap/Minimap.hpp

    Partner/Drone.cpp
    Partner/Drone.hpp

    AI/ChatModel.cpp
    AI/ChatModel.hpp

    main.cpp
)

IF (WIN32)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/include)
    target_link_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/lib)
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC lib)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense ${ALLEGRO5_LIBRARIES})
    target_link_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()

# === ONNX Runtime Begin ===
# 設定路徑
# set(ONNX_RUNTIME_DIR "C:/onnxruntime")

# target_include_directories(2025_I2P2_TowerDefense PUBLIC "${ONNX_RUNTIME_DIR}/include")

# # 直接連結靜態庫或匯入庫
# target_link_libraries(2025_I2P2_TowerDefense "${ONNX_RUNTIME_DIR}/lib/onnxruntime.lib")

# # 複製 dll 到執行目錄
# if(WIN32)
#     add_custom_command(TARGET 2025_I2P2_TowerDefense POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "${ONNX_RUNTIME_DIR}/lib/onnxruntime.dll"
#         $<TARGET_FILE_DIR:2025_I2P2_TowerDefense>
#     )
# endif()

# === ONNX Runtime End ===


add_subdirectory(llama)

target_include_directories(2025_I2P2_TowerDefense PRIVATE llama/include)
target_link_libraries(2025_I2P2_TowerDefense llama)