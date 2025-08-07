import os

from Gaudi.Configuration import INFO
from k4FWCore import IOSvc, ApplicationMgr
from Configurables import EventDataSvc, GeoSvc

io_svc = IOSvc("IOSvc")
io_svc.Input = "../data/simplecalo1_example.root"
io_svc.Output = "../data/output.root"

from Configurables import MoliereRadiusSolution
moliere_radius_solution = MoliereRadiusSolution("MoliereRadiusSolution",
                                                InputCaloHitCollection=["simplecaloRO"])

from Configurables import MoliereRadius
moliere_radius = MoliereRadius("MoliereRadius",
                                InputCaloHitCollection=["simplecaloRO"])

app_mgr = ApplicationMgr(
    TopAlg=[moliere_radius_solution, moliere_radius],
    EvtSel='NONE',
    EvtMax=-1,
    ExtSvc=[EventDataSvc("EventDataSvc")],
    StopOnSignal=True,
)