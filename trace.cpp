#include <iostream>
#include <cmath>

class PlayerBullet {
public:
    PlayerBullet(float _speed, float _homingTime) :
        speed(_speed), homingTime(_homingTime), target(nullptr), timer(0.f)
    {}

    void Update(float deltaTime) {
        if (target == nullptr) { // 如果目標為空，就直接前進
            x += speed * deltaTime * std::sin(angle);
            y += speed * deltaTime * std::cos(angle);
        }
        else { // 否則追踪目標
            float dx = target->GetX() - x;
            float dy = target->GetY() - y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist > 0.f) {
                float vx = speed * dx / dist;
                float vy = speed * dy / dist;
                x += vx * deltaTime;
                y += vy * deltaTime;
            }
        }

        timer += deltaTime;
        if (timer >= homingTime) { // 追踪時間到，子彈消失
            Destroy();
        }
    }

    void SetTarget(Enemy* _target) {
        target = _target;
    }

    void OnTriggerEnter(Enemy* enemy) {
        if (enemy == target) { // 碰到敵人，子彈消失
            Destroy();
        }
    }

private:
    float speed;
    float homingTime;
    float x;
    float y;
    float angle;
    Enemy* target;
    float timer;

    void Destroy() {
        delete this;
    }
};

class Player {
public:
    Player() :
        bulletSpeed(10.f), bulletHomingTime(5.f), target(nullptr)
    {}

    void Update(float deltaTime) {
        if (IsSpacePressed()) {
            SpawnBullet();
        }
    }

    void SetTarget(Enemy* enemy) {
        target = enemy;
    }

    void ClearTarget() {
        target = nullptr;
    }

private:
    float bulletSpeed;
    float bulletHomingTime;
    Enemy* target;

    bool IsSpacePressed() {
        // 檢查是否按下空白鍵
        return false;
    }

    void SpawnBullet() {
        PlayerBullet* bullet = new PlayerBullet(bulletSpeed, bulletHomingTime);
        bullet->SetTarget(target);
        // 生成子彈
    }
};

class Enemy {
public:
    float GetX() const {
        return x;
    }

    float GetY() const {
        return y;
    }

private:
    float x;
    float y;
};

int main() {
    // 初始化遊戲對象
    Player player;
    Enemy enemy;

    while (true) {
        // 更新遊戲狀態
        float deltaTime = 0.016f;
        player.Update(deltaTime);
        player.SetTarget(&enemy);
        enemy.Update(deltaTime);
        player.ClearTarget();
    }

    return 0;
}
