#include <DX3D/UI/WorldOutliner.h>
#include <DX3D/Game/Game.h>
#include <imgui.h>

void dx3d::WorldOutliner::render(
    std::vector<GameObjectPtr>& objects,
    GameObjectPtr& selectedObject,
    Game* gameContext)
{
    (void)gameContext;
    if (!m_isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(280, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(980, 50), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("World Outliner (Scene Hierarchy)", &m_isOpen))
    {
        ImGui::InputTextWithHint("##Filter", "Search Actors...", m_searchFilter, sizeof(m_searchFilter));
        ImGui::Spacing();
        ImGui::Separator();

        ImGui::BeginChild("ActorList", ImVec2(0, -35), true);
        for (size_t i = 0; i < objects.size(); ++i)
        {
            auto& obj = objects[i];
            if (!obj) continue;

            if (m_searchFilter[0] != '\0')
            {
                if (obj->m_name.find(m_searchFilter) == std::string::npos)
                    continue;
            }

            bool isSelected = (selectedObject == obj);
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
            if (isSelected) flags |= ImGuiTreeNodeFlags_Selected;

            std::string label = obj->m_name;
            if (obj->m_rigidBody)
            {
                label += " [" + std::to_string(static_cast<int>(obj->m_rigidBody->m_mass)) + "kg]";
                if (obj->m_rigidBody->m_isStatic) label += " (Static)";
            }

            if (ImGui::TreeNodeEx((void*)(intptr_t)i, flags, "%s", label.c_str()))
            {
                if (ImGui::IsItemClicked())
                {
                    selectedObject = obj;
                }
            }
        }
        ImGui::EndChild();

        if (selectedObject)
        {
            if (ImGui::Button("Delete Selected", ImVec2(-1, 24)))
            {
                std::erase(objects, selectedObject);
                selectedObject = nullptr;
            }
        }
    }
    ImGui::End();
}
