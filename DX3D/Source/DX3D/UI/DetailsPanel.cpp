#include <DX3D/UI/DetailsPanel.h>
#include <DX3D/Game/Game.h>
#include <imgui.h>
#include <algorithm>
#include <cmath>

void dx3d::DetailsPanel::render(GameObject* selectedObject, Game* gameContext)
{
    if (!m_isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(340, 580), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(10, 50), ImGuiCond_FirstUseEver);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    if (ImGui::Begin("Details ", &m_isOpen, flags))
    {
        if (!selectedObject)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
            ImGui::TextWrapped("No Actor selected. Click a cube in the 3D viewport to inspect and modify its properties.");
            ImGui::PopStyleColor();

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("Quick Add New Cube:");
            if (ImGui::Button("+ Spawn Standard Cube (1m)", ImVec2(-1, 30)))
            {
                if (m_spawnCallback)
                    m_spawnCallback({ 0.0f, 4.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f, true, { 0.2f, 0.6f, 1.0f });
            }
            if (ImGui::Button("+ Spawn Heavy Block (25kg)", ImVec2(-1, 30)))
            {
                if (m_spawnCallback)
                    m_spawnCallback({ 0.0f, 6.0f, 0.0f }, { 1.8f, 1.8f, 1.8f }, 25.0f, true, { 1.0f, 0.5f, 0.1f });
            }
            if (ImGui::Button("+ Spawn Bouncy Cube (e=0.9)", ImVec2(-1, 30)))
            {
                if (m_spawnCallback)
                    m_spawnCallback({ 0.0f, 5.0f, 0.0f }, { 1.2f, 1.2f, 1.2f }, 2.0f, true, { 0.3f, 0.9f, 0.4f });
            }
            ImGui::End();
            return;
        }

        char nameBuffer[64];
        strncpy_s(nameBuffer, selectedObject->m_name.c_str(), sizeof(nameBuffer));
        if (ImGui::InputText("Actor Name", nameBuffer, sizeof(nameBuffer)))
        {
            selectedObject->m_name = nameBuffer;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Active", &selectedObject->m_active);

        ImGui::Spacing();
        ImGui::Separator();

        renderTransformSection(selectedObject);

        ImGui::Spacing();
        ImGui::Separator();

        renderPhysicsSection(selectedObject);

        ImGui::Spacing();
        ImGui::Separator();

        renderMaterialSection(selectedObject);

        ImGui::Spacing();
        ImGui::Separator();

        renderQuickActionsSection(selectedObject, gameContext);
    }
    ImGui::End();
}

void dx3d::DetailsPanel::renderTransformSection(GameObject* obj)
{
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Location (Meters):");
        float pos[3] = { obj->m_position.x, obj->m_position.y, obj->m_position.z };
        if (ImGui::DragFloat3("##Location", pos, 0.05f, -100.0f, 100.0f, "%.2f m"))
        {
            obj->m_position = { pos[0], pos[1], pos[2] };
        }

        ImGui::TextColored(ImVec4(0.4f, 0.9f, 1.0f, 1.0f), "Rotation (Degrees):");
        float rotDeg[3] = {
            obj->m_rotation.x * 57.29578f,
            obj->m_rotation.y * 57.29578f,
            obj->m_rotation.z * 57.29578f
        };
        if (ImGui::DragFloat3("##Rotation", rotDeg, 1.0f, -360.0f, 360.0f, "%.1f deg"))
        {
            obj->m_rotation = {
                rotDeg[0] * 0.01745329f,
                rotDeg[1] * 0.01745329f,
                rotDeg[2] * 0.01745329f
            };
        }

        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Scale / Size (Adjuster):");
        ImGui::Checkbox("Uniform Scale", &m_uniformScale);

        if (m_uniformScale)
        {
            float uniformS = (obj->m_scale.x + obj->m_scale.y + obj->m_scale.z) / 3.0f;
            if (ImGui::SliderFloat("Uniform Size", &uniformS, 0.1f, 10.0f, "%.2f"))
            {
                obj->setScale({ uniformS, uniformS, uniformS });
            }
        }

        float scale[3] = { obj->m_scale.x, obj->m_scale.y, obj->m_scale.z };
        if (ImGui::DragFloat3("Size (X,Y,Z)", scale, 0.05f, 0.1f, 20.0f, "%.2f"))
        {
            obj->setScale({ (std::max)(0.1f, scale[0]), (std::max)(0.1f, scale[1]), (std::max)(0.1f, scale[2]) });
        }

        if (ImGui::Button("Reset Transform", ImVec2(-1, 24)))
        {
            obj->m_position = { 0.0f, 2.0f, 0.0f };
            obj->m_rotation = { 0.0f, 0.0f, 0.0f };
            obj->setScale({ 1.0f, 1.0f, 1.0f });
            if (obj->m_rigidBody)
            {
                obj->m_rigidBody->m_velocity = { 0.0f, 0.0f, 0.0f };
                obj->m_rigidBody->m_angularVelocity = { 0.0f, 0.0f, 0.0f };
            }
        }
    }
}

void dx3d::DetailsPanel::renderPhysicsSection(GameObject* obj)
{
    if (ImGui::CollapsingHeader("Physics & Dynamics", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto& rb = obj->m_rigidBody;
        if (!rb)
        {
            ImGui::TextDisabled("No RigidBody attached.");
            return;
        }

        bool isStatic = rb->m_isStatic;
        if (ImGui::Checkbox("Static Object (Immovable)", &isStatic))
        {
            rb->setStatic(isStatic);
        }

        ImGui::Checkbox("Enable Gravity (Simulate Gravity)", &rb->m_useGravity);

        ImGui::Spacing();
        ImGui::Text("Mass / Weight (Kg):");
        float mass = rb->m_mass;
        if (ImGui::SliderFloat("##MassSlider", &mass, 0.1f, 100.0f, "%.2f kg", ImGuiSliderFlags_Logarithmic))
        {
            rb->setMass(mass);
        }
        if (ImGui::InputFloat("Mass Value (kg)", &mass, 0.5f, 5.0f, "%.2f"))
        {
            rb->setMass((std::max)(0.01f, mass));
        }

        ImGui::Spacing();
        ImGui::SliderFloat("Restitution (Bounciness)", &rb->m_restitution, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Friction Coefficient", &rb->m_friction, 0.0f, 1.0f, "%.2f");

        ImGui::SliderFloat("Linear Damping", &rb->m_linearDamping, 0.0f, 0.5f, "%.3f");
        ImGui::SliderFloat("Angular Damping", &rb->m_angularDamping, 0.0f, 0.5f, "%.3f");

        ImGui::Spacing();
        ImGui::TextDisabled("Live Physics Telemetry:");
        ImGui::Text("Velocity: (%.2f, %.2f, %.2f) m/s", rb->m_velocity.x, rb->m_velocity.y, rb->m_velocity.z);
        ImGui::Text("Speed: %.2f m/s", rb->m_velocity.length());
        ImGui::Text("Kinetic Energy: %.2f J", rb->getKineticEnergy());
    }
}

void dx3d::DetailsPanel::renderMaterialSection(GameObject* obj)
{
    if (ImGui::CollapsingHeader("Material & Color", ImGuiTreeNodeFlags_DefaultOpen))
    {
        float col[3] = { obj->m_color.x, obj->m_color.y, obj->m_color.z };
        if (ImGui::ColorEdit3("Base Color", col))
        {
            obj->m_color = { col[0], col[1], col[2] };
        }

        ImGui::Text("Presets:");
        ImGui::SameLine();
        if (ImGui::ColorButton("Blue", ImVec4(0.2f, 0.6f, 1.0f, 1.0f), 0, ImVec2(24, 24))) obj->m_color = { 0.2f, 0.6f, 1.0f };
        ImGui::SameLine();
        if (ImGui::ColorButton("Red", ImVec4(1.0f, 0.3f, 0.2f, 1.0f), 0, ImVec2(24, 24))) obj->m_color = { 1.0f, 0.3f, 0.2f };
        ImGui::SameLine();
        if (ImGui::ColorButton("Green", ImVec4(0.2f, 0.9f, 0.4f, 1.0f), 0, ImVec2(24, 24))) obj->m_color = { 0.2f, 0.9f, 0.4f };
        ImGui::SameLine();
        if (ImGui::ColorButton("Yellow", ImVec4(1.0f, 0.8f, 0.1f, 1.0f), 0, ImVec2(24, 24))) obj->m_color = { 1.0f, 0.8f, 0.1f };
        ImGui::SameLine();
        if (ImGui::ColorButton("Purple", ImVec4(0.8f, 0.3f, 0.9f, 1.0f), 0, ImVec2(24, 24))) obj->m_color = { 0.8f, 0.3f, 0.9f };
    }
}

void dx3d::DetailsPanel::renderQuickActionsSection(GameObject* obj, Game* gameContext)
{
    (void)gameContext;
    if (ImGui::CollapsingHeader("Test Forces & Impulses", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto& rb = obj->m_rigidBody;
        if (!rb || rb->m_isStatic)
        {
            ImGui::TextDisabled("Enable movable physics to apply impulses.");
            return;
        }

        if (ImGui::Button("Jump Upward Impulse (+Y)", ImVec2(-1, 26)))
        {
            rb->applyImpulse({ 0.0f, 8.0f * rb->m_mass, 0.0f });
        }
        if (ImGui::Button("Kick Forward (+Z)", ImVec2(-1, 26)))
        {
            rb->applyImpulse({ 0.0f, 3.0f * rb->m_mass, 6.0f * rb->m_mass });
        }
        if (ImGui::Button("Add Spin Torque", ImVec2(-1, 26)))
        {
            rb->m_angularVelocity += Vec3{ 2.0f, 4.0f, 1.0f };
        }
        if (ImGui::Button("Zero Velocity (Halt)", ImVec2(-1, 26)))
        {
            rb->m_velocity = { 0.0f, 0.0f, 0.0f };
            rb->m_angularVelocity = { 0.0f, 0.0f, 0.0f };
        }
    }
}
