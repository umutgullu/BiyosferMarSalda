import bpy
import math

bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete(use_global=False)

def apply_smooth(obj, angle_degrees=30.0):
    bpy.context.view_layer.objects.active = obj
    bpy.ops.object.shade_smooth()
    if hasattr(obj.data, "use_auto_smooth"):
        obj.data.use_auto_smooth = True
        obj.data.auto_smooth_angle = math.radians(angle_degrees)
    else:
        mod = obj.modifiers.new(name="Smooth_by_Angle", type='NODES')
        node_group = bpy.data.node_groups.get("Smooth by Angle")
        if node_group:
            mod.node_group = node_group

ic_cap = 0.20         
ic_yaricap = ic_cap / 2.0  
ic_yukseklik = 0.20   

et_kalinligi = 0.006  
bosluk_payi = 0.014   
cekmece_yukseklik = 0.05 

dis_yaricap = ic_yaricap + et_kalinligi + bosluk_payi 
dis_yukseklik = ic_yukseklik + et_kalinligi + cekmece_yukseklik 

dis_merkez_z = dis_yukseklik / 2.0
ic_merkez_z = dis_yukseklik - (ic_yukseklik / 2.0)

bpy.ops.mesh.primitive_cylinder_add(vertices=128, radius=dis_yaricap, depth=dis_yukseklik, location=(0, 0, dis_merkez_z))
govde = bpy.context.active_object
govde.name = "SmartPlanter_Govde"

bpy.ops.mesh.primitive_cylinder_add(vertices=128, radius=ic_yaricap, depth=ic_yukseklik + 0.002, location=(0, 0, ic_merkez_z + 0.001))
ic_oyuk = bpy.context.active_object

bool_ic = govde.modifiers.new(name="Ic_Bosluk", type='BOOLEAN')
bool_ic.operation = 'DIFFERENCE'
bool_ic.solver = 'EXACT'
bool_ic.object = ic_oyuk
bpy.context.view_layer.objects.active = govde
bpy.ops.object.modifier_apply(modifier="Ic_Bosluk")
bpy.data.objects.remove(ic_oyuk, do_unlink=True)

for i in range(7):
    angle = (i * 51.4) * (math.pi / 180)
    delik_x = math.cos(angle) * (ic_yaricap * 0.5)
    delik_y = math.sin(angle) * (ic_yaricap * 0.5)
    delik_z = ic_merkez_z - (ic_yukseklik / 2.0)
    
    bpy.ops.mesh.primitive_cylinder_add(vertices=16, radius=0.004, depth=0.04, location=(delik_x, delik_y, delik_z))
    delik = bpy.context.active_object
    bool_delik = govde.modifiers.new(name=f"Delik_{i}", type='BOOLEAN')
    bool_delik.operation = 'DIFFERENCE'
    bool_delik.solver = 'EXACT'
    bool_delik.object = delik
    bpy.context.view_layer.objects.active = govde
    bpy.ops.object.modifier_apply(modifier=f"Delik_{i}")
    bpy.data.objects.remove(delik, do_unlink=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, -0.06, 0.026))
yuva_kesici = bpy.context.active_object
yuva_kesici.scale = (0.12, 0.14, 0.045)

bool_yuva = govde.modifiers.new(name="Cekmece_Yuvasi", type='BOOLEAN')
bool_yuva.operation = 'DIFFERENCE'
bool_yuva.solver = 'EXACT'
bool_yuva.object = yuva_kesici
bpy.context.view_layer.objects.active = govde
bpy.ops.object.modifier_apply(modifier="Cekmece_Yuvasi")
bpy.data.objects.remove(yuva_kesici, do_unlink=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, -0.3, 0.025))
cekmece = bpy.context.active_object
cekmece.scale = (0.1192, 0.1392, 0.0442)
cekmece.name = "SmartPlanter_Cekmece"

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, -0.3, 0.028))
cekmece_ici = bpy.context.active_object
cekmece_ici.scale = (0.113, 0.133, 0.04)

bool_cekmece_ic = cekmece.modifiers.new(name="Cekmece_Ici", type='BOOLEAN')
bool_cekmece_ic.operation = 'DIFFERENCE'
bool_cekmece_ic.solver = 'EXACT'
bool_cekmece_ic.object = cekmece_ici
bpy.context.view_layer.objects.active = cekmece
bpy.ops.object.modifier_apply(modifier="Cekmece_Ici")
bpy.data.objects.remove(cekmece_ici, do_unlink=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, -0.372, 0.025))
kulp = bpy.context.active_object
kulp.scale = (0.04, 0.005, 0.01)

bool_kulp = cekmece.modifiers.new(name="Kulp_Birlestir", type='BOOLEAN')
bool_kulp.operation = 'UNION'
bool_kulp.solver = 'EXACT'
bool_kulp.object = kulp
bpy.context.view_layer.objects.active = cekmece
bpy.ops.object.modifier_apply(modifier="Kulp_Birlestir")
bpy.data.objects.remove(kulp, do_unlink=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0.115, 0, 0.15))
ekran_kesici = bpy.context.active_object
ekran_kesici.scale = (0.02, 0.06, 0.04)

bool_ekran = govde.modifiers.new(name="Ekran_Centigi", type='BOOLEAN')
bool_ekran.operation = 'DIFFERENCE'
bool_ekran.solver = 'EXACT'
bool_ekran.object = ekran_kesici
bpy.context.view_layer.objects.active = govde
bpy.ops.object.modifier_apply(modifier="Ekran_Centigi")
bpy.data.objects.remove(ekran_kesici, do_unlink=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, 0.115, 0.025))
usb_kesici = bpy.context.active_object
usb_kesici.scale = (0.02, 0.02, 0.01)

bool_usb = govde.modifiers.new(name="USB_Port", type='BOOLEAN')
bool_usb.operation = 'DIFFERENCE'
bool_usb.solver = 'EXACT'
bool_usb.object = usb_kesici
bpy.context.view_layer.objects.active = govde
bpy.ops.object.modifier_apply(modifier="USB_Port")
bpy.data.objects.remove(usb_kesici, do_unlink=True)

apply_smooth(govde, 30.0)
apply_smooth(cekmece, 30.0)

altlik_oturtma_toleransi = 0.0015
altlik_et_kalinligi = 0.004
altlik_derinlik = 0.025

altlik_ic_yaricap = dis_yaricap + altlik_oturtma_toleransi
altlik_dis_yaricap = altlik_ic_yaricap + altlik_et_kalinligi
altlik_toplam_yukseklik = altlik_derinlik + altlik_et_kalinligi

altlik_pos_x = 0.35
altlik_merkez_z = altlik_toplam_yukseklik / 2.0

bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=altlik_dis_yaricap,
    depth=altlik_toplam_yukseklik,
    location=(altlik_pos_x, 0, altlik_merkez_z)
)
altlik = bpy.context.active_object
altlik.name = "SmartPlanter_Altlik"

kesici_yukseklik = altlik_derinlik + 0.010
kesici_merkez_z = altlik_et_kalinligi + (kesici_yukseklik / 2.0)

bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=altlik_ic_yaricap,
    depth=kesici_yukseklik,
    location=(altlik_pos_x, 0, kesici_merkez_z)
)
altlik_ic_oyuk = bpy.context.active_object

bool_altlik = altlik.modifiers.new(name="Altlik_Ic_Bosluk", type='BOOLEAN')
bool_altlik.operation = 'DIFFERENCE'
bool_altlik.solver = 'EXACT'
bool_altlik.object = altlik_ic_oyuk
bpy.context.view_layer.objects.active = altlik
bpy.ops.object.modifier_apply(modifier="Altlik_Ic_Bosluk")
bpy.data.objects.remove(altlik_ic_oyuk, do_unlink=True)

apply_smooth(altlik, 30.0)

kemer_dis_cap = 0.238
kemer_ic_cap = 0.203
kemer_yukseklik = 0.040

kemer_dis_yaricap = kemer_dis_cap / 2.0
kemer_ic_yaricap = kemer_ic_cap / 2.0
kemer_merkez_z = kemer_yukseklik / 2.0
kemer_pos_x = -0.35

bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=kemer_dis_yaricap,
    depth=kemer_yukseklik,
    location=(kemer_pos_x, 0, kemer_merkez_z)
)
kemer = bpy.context.active_object
kemer.name = "SmartPlanter_LedKemeri"

bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=kemer_ic_yaricap,
    depth=kemer_yukseklik + 0.006,
    location=(kemer_pos_x, 0, kemer_merkez_z)
)
kemer_ic_kesici = bpy.context.active_object

bool_kemer_ic = kemer.modifiers.new(name="Kemer_Ic_Oyuk", type='BOOLEAN')
bool_kemer_ic.operation = 'DIFFERENCE'
bool_kemer_ic.solver = 'EXACT'
bool_kemer_ic.object = kemer_ic_kesici
bpy.context.view_layer.objects.active = kemer
bpy.ops.object.modifier_apply(modifier="Kemer_Ic_Oyuk")
bpy.data.objects.remove(kemer_ic_kesici, do_unlink=True)

led_kanal_yaricap = kemer_ic_yaricap + 0.004
led_kanal_yukseklik = 0.014

bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=led_kanal_yaricap,
    depth=led_kanal_yukseklik,
    location=(kemer_pos_x, 0, kemer_merkez_z)
)
led_kanal_kesici = bpy.context.active_object

bool_led = kemer.modifiers.new(name="LED_Kanal_Oyugu", type='BOOLEAN')
bool_led.operation = 'DIFFERENCE'
bool_led.solver = 'EXACT'
bool_led.object = led_kanal_kesici
bpy.context.view_layer.objects.active = kemer
bpy.ops.object.modifier_apply(modifier="LED_Kanal_Oyugu")
bpy.data.objects.remove(led_kanal_kesici, do_unlink=True)

bpy.ops.mesh.primitive_cylinder_add(
    vertices=16,
    radius=0.0025,
    depth=0.040,
    location=(kemer_pos_x + kemer_dis_yaricap - 0.01, 0, kemer_merkez_z),
    rotation=(0, 1.5708, 0)
)
kablo_deligi = bpy.context.active_object

bool_kablo = kemer.modifiers.new(name="Kablo_Deligi", type='BOOLEAN')
bool_kablo.operation = 'DIFFERENCE'
bool_kablo.solver = 'EXACT'
bool_kablo.object = kablo_deligi
bpy.context.view_layer.objects.active = kemer
bpy.ops.object.modifier_apply(modifier="Kablo_Deligi")
bpy.data.objects.remove(kablo_deligi, do_unlink=True)

apply_smooth(kemer, 30.0)

def create_clip(name, loc_x, loc_y, loc_z):
    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x, loc_y, loc_z))
    klips = bpy.context.active_object
    klips.name = name
    klips.scale = (0.030, 0.025, 0.038)
    bpy.ops.object.transform_apply(scale=True)

    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x - 0.003, loc_y, loc_z))
    kesici_yay = bpy.context.active_object
    kesici_yay.scale = (0.016, 0.027, 0.026)
    bpy.ops.object.transform_apply(scale=True)

    bool_yay = klips.modifiers.new(name="Yay_Boslugu", type='BOOLEAN')
    bool_yay.operation = 'DIFFERENCE'
    bool_yay.solver = 'EXACT'
    bool_yay.object = kesici_yay
    bpy.context.view_layer.objects.active = klips
    bpy.ops.object.modifier_apply(modifier="Yay_Boslugu")
    bpy.data.objects.remove(kesici_yay, do_unlink=True)

    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x + 0.010, loc_y, loc_z))
    kesici_led = bpy.context.active_object
    kesici_led.scale = (0.006, 0.027, 0.014)
    bpy.ops.object.transform_apply(scale=True)

    bool_led_klips = klips.modifiers.new(name="LED_Korumasi", type='BOOLEAN')
    bool_led_klips.operation = 'DIFFERENCE'
    bool_led_klips.solver = 'EXACT'
    bool_led_klips.object = kesici_led
    bpy.context.view_layer.objects.active = klips
    bpy.ops.object.modifier_apply(modifier="LED_Korumasi")
    bpy.data.objects.remove(kesici_led, do_unlink=True)

    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x + 0.012, loc_y, loc_z - 0.019))
    tirnak = bpy.context.active_object
    tirnak.scale = (0.005, 0.025, 0.004)
    bpy.ops.object.transform_apply(scale=True)

    bool_tirnak = klips.modifiers.new(name="Kemer_Tirnagi", type='BOOLEAN')
    bool_tirnak.operation = 'UNION'
    bool_tirnak.solver = 'EXACT'
    bool_tirnak.object = tirnak
    bpy.context.view_layer.objects.active = klips
    bpy.ops.object.modifier_apply(modifier="Kemer_Tirnagi")
    bpy.data.objects.remove(tirnak, do_unlink=True)

    bpy.ops.mesh.primitive_cylinder_add(
        vertices=32,
        radius=0.003,
        depth=0.025,
        location=(loc_x - 0.011, loc_y, loc_z - 0.004),
        rotation=(1.5708, 0, 0)
    )
    burun = bpy.context.active_object
    bpy.ops.object.transform_apply(rotation=True)

    bool_burun = klips.modifiers.new(name="Temas_Burnu", type='BOOLEAN')
    bool_burun.operation = 'UNION'
    bool_burun.solver = 'EXACT'
    bool_burun.object = burun
    bpy.context.view_layer.objects.active = klips
    bpy.ops.object.modifier_apply(modifier="Temas_Burnu")
    bpy.data.objects.remove(burun, do_unlink=True)

    apply_smooth(klips, 30.0)

klips_merkez_z = 0.038 / 2.0
aralik = 0.035

for i in range(4):
    pos_y = (i - 1.5) * aralik
    create_clip(f"SmartPlanter_Klips_{i+1}", 0, 0.35 + pos_y, klips_merkez_z)

print("Tam set basima hazir sekilde basariyla modellendi.")
