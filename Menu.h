#pragma once


void DrawThreadInformation() 
{
    ImVec2 pos(1300, 50);
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);

    ImGui::Columns(3, "import_columns", true);

    ImGui::Text("DLL Name"); ImGui::NextColumn();
    ImGui::Text("Function Name"); ImGui::NextColumn();
    ImGui::Text("Function Address"); ImGui::NextColumn();
    ImGui::Separator();

    for (const auto& info : imports) {
        ImGui::Text(info.dllName.c_str()); ImGui::NextColumn();
        ImGui::Text(info.functionName.c_str()); ImGui::NextColumn();
        ImGui::Text("%p", info.functionAddress); ImGui::NextColumn();
    }

    ImGui::End();
}

<<<<<<< HEAD

void DrawImports()
{
    ImVec2 pos(700, 50);
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);

    ImGui::Begin("[ReverseKit] Imports");
=======
void DrawThreadInformation()
{
    ImGui::Begin("[ReverseKit] Active Threads");
>>>>>>> 6f688fee0e0be9eb6a8149542a5cd200abf711f5
    ImGui::SetWindowSize(ImVec2(600, 400), ImGuiCond_Once);

    ImGui::Columns(3, "import_columns", true);
    ImGui::Text("DLL Name"); ImGui::NextColumn();
    ImGui::Text("Function Name"); ImGui::NextColumn();
    ImGui::Text("Function Address"); ImGui::NextColumn();
    ImGui::Separator();

    for (const auto& info : imports) {
        ImGui::Text(info.dllName.c_str()); ImGui::NextColumn();
        ImGui::Text(info.functionName.c_str()); ImGui::NextColumn();
        ImGui::Text("%p", info.functionAddress); ImGui::NextColumn();
    }

    ImGui::End();
}

void DrawHookedFunctions()
{
    ImVec2 pos(300, 50);
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);

    ImGui::Begin("[ReverseKit] Hooked Functions");
    ImGui::SetWindowSize(ImVec2(400, 600), ImGuiCond_Once);

    static std::unordered_map<std::string, std::vector<InterceptedCallInfo>> functionCalls;
    for (const auto& call : interceptedCalls)
    {
        functionCalls[call.functionName].push_back(call);
    }

    for (const auto& pair : functionCalls)
    {
        const std::string& functionName = pair.first;
        const std::vector<InterceptedCallInfo>& calls = pair.second;

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::CollapsingHeader(functionName.c_str()))
        {
            for (const auto& call : calls)
            {
                if (ImGui::Selectable(call.additionalInfo.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    ImGui::SetClipboardText(call.additionalInfo.c_str());
                }
            }
        }
    }

    functionCalls.clear();

    ImGui::End();
}

void RenderUI()
{
    DrawImports();
    DrawThreadInformation();
    DrawHookedFunctions();
}
