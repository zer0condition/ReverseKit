#pragma once

void DrawImports()
{
    ImGui::Begin("[ReverseKit] Imports");
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

void DrawThreadInformation()
{
    ImGui::Begin("[ReverseKit] Active Threads");
    ImGui::SetWindowSize(ImVec2(600, 400), ImGuiCond_Once);

    ImGui::Columns(3, "thread_columns", true);

    ImGui::Text("Thread ID"); ImGui::NextColumn();
    ImGui::Text("CPU Usage"); ImGui::NextColumn();
    ImGui::Text(""); ImGui::NextColumn();
    ImGui::Separator();

    for (auto& info : threadInfo) {
        ImGui::Text("%lu", info.threadId); ImGui::NextColumn();
        ImGui::Text("%u%%", info.cpuUsage); ImGui::NextColumn();

        if (ImGui::Button("Suspend")) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, info.threadId);
            if (hThread != NULL) {
                SuspendThread(hThread);
                CloseHandle(hThread);
            }
        }
        ImGui::NextColumn();
    }

    ImGui::End();
}

void DrawHookedFunctions()
{
    ImGui::Begin("[ReverseKit] Hooked Functions");
    ImGui::SetWindowSize(ImVec2(400, 600), ImGuiCond_Once);

    // Loop through the intercepted calls and group them by function name
    static std::unordered_map<std::string, std::vector<InterceptedCallInfo>> functionCalls;
    for (const auto& call : interceptedCalls)
    {
        functionCalls[call.functionName].push_back(call);
    }

    for (const auto& pair : functionCalls)
    {
        const std::string& functionName = pair.first;
        const std::vector<InterceptedCallInfo>& calls = pair.second;

        // Add a new collapsible header for this function
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::CollapsingHeader(functionName.c_str()))
        {
            // Display the additional information for each intercepted call
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
