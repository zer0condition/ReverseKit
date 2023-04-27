#pragma once

inline void DrawInstrumentationInformation() {
    ImGui::Begin("[ReverseKit] Instrumentation");
    ImGui::SetWindowSize(ImVec2(600, 600), ImGuiCond_Once);

    if (ImGui::Button("Clear")) {
        function_calls.clear();
    }

    ImGui::Text("Press F1 to log system calls"); 
    ImGui::Columns(2, "instrumentation_columns", true);

    ImGui::Text("Function Name"); ImGui::NextColumn();
    ImGui::Text("Return Address"); ImGui::NextColumn();
    ImGui::Separator();

    for (auto& call_info : function_calls) {
        ImGui::Text("%s", call_info.function_name.c_str()); ImGui::NextColumn();
        ImGui::Text("%p", call_info.return_address); ImGui::NextColumn();
    }

    ImGui::End();
}

inline void DrawThreadInformation()
{
    ImGui::Begin("[ReverseKit] Active Threads");
    ImGui::SetWindowSize(ImVec2(600, 400), ImGuiCond_Once);

    ImGui::Columns(3, "thread_columns", true);

    ImGui::Text("Thread ID"); ImGui::NextColumn();
    ImGui::Text("CPU Usage"); ImGui::NextColumn();
    ImGui::Text(""); ImGui::NextColumn();
    ImGui::Separator();

    for (const auto& info : threadInfo) {
        ImGui::Text("%lu", info.threadId); ImGui::NextColumn();
        ImGui::Text("%u%%", info.cpuUsage); ImGui::NextColumn();

        if (ImGui::Button("Suspend")) {
	        const HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, info.threadId);
            if (hThread != nullptr) {
                SuspendThread(hThread);
                CloseHandle(hThread);
            }
        }
        ImGui::NextColumn();
    }

    ImGui::End();
}

inline void DrawImports()
{
    ImGui::Begin("[ReverseKit] Imports");
    ImGui::SetWindowSize(ImVec2(600, 600), ImGuiCond_Once);

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

inline void DrawHookedFunctions()
{
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
    static bool showThreads = false;
    static bool showImports = true;
    static bool showHookedFunctions = false;
    static bool showInstrumentation = false;

    RECT rect{};
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    const int screenWidth = rect.right - rect.left;
    [[maybe_unused]] int screenHeight = rect.bottom - rect.top;
    const ImVec2 windowSize(390, 0);

    ImGui::SetNextWindowPos(ImVec2((screenWidth / 2) - (windowSize.x / 2), 0));
    ImGui::SetNextWindowSize(windowSize);

    ImGui::Begin("[ReverseKit] Main Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    const ImVec4 activeButtonColor = ImVec4(0.2f, 0.6f, 1.0f, 1.0f);
    const ImVec4 inactiveButtonColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

    if (showImports)
        ImGui::PushStyleColor(ImGuiCol_Button, activeButtonColor);
    else
        ImGui::PushStyleColor(ImGuiCol_Button, inactiveButtonColor);

    if (ImGui::Button("Imports"))
        showImports = !showImports;

    ImGui::PopStyleColor();

    ImGui::SameLine();

    if (showHookedFunctions)
        ImGui::PushStyleColor(ImGuiCol_Button, activeButtonColor);
    else
        ImGui::PushStyleColor(ImGuiCol_Button, inactiveButtonColor);

    if (ImGui::Button("Hooked Functions"))
        showHookedFunctions = !showHookedFunctions;

    ImGui::PopStyleColor();

    ImGui::SameLine();

    if (showThreads)
        ImGui::PushStyleColor(ImGuiCol_Button, activeButtonColor);
    else
        ImGui::PushStyleColor(ImGuiCol_Button, inactiveButtonColor);

    if (ImGui::Button("Threads"))
        showThreads = !showThreads;

    ImGui::PopStyleColor();

    ImGui::SameLine();

    if (showInstrumentation)
        ImGui::PushStyleColor(ImGuiCol_Button, activeButtonColor);
    else
        ImGui::PushStyleColor(ImGuiCol_Button, inactiveButtonColor);

    if (ImGui::Button("Instrumentation"))
        showInstrumentation = !showInstrumentation;

    ImGui::PopStyleColor();

    ImGui::End();

    if (showInstrumentation)
        DrawInstrumentationInformation();
    if (showThreads)
        DrawThreadInformation();
    if (showImports)
        DrawImports();
    if (showHookedFunctions)
        DrawHookedFunctions();
}
