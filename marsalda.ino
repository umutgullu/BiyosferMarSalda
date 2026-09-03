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
cekmece_alani_yukseklik = 0.065 

dis_yaricap = ic_yaricap + et_kalinligi + bosluk_payi
dis_yukseklik = ic_yukseklik + et_kalinligi + cekmece_alani_yukseklik 

dis_merkez_z = dis_yukseklik / 2.0
ic_merkez_z = dis_yukseklik - (ic_yukseklik / 2.0)

# ==========================================
# 1. ANA GOVDE VE ENTEGRE CAM OTURMA YUVASI
# ==========================================
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

cam_yuva_yaricap = 0.102  
cam_yuva_derinlik = 0.008
cam_yuva_z = dis_yukseklik - (cam_yuva_derinlik / 2.0) + 0.001

bpy.ops.mesh.primitive_cylinder_add(vertices=128, radius=cam_yuva_yaricap, depth=cam_yuva_derinlik + 0.002, location=(0, 0, cam_yuva_z))
cam_yuva_kesici = bpy.context.active_object

bool_cam_yuva = govde.modifiers.new(name="Cam_Oturma_Yuvasi", type='BOOLEAN')
bool_cam_yuva.operation = 'DIFFERENCE'
bool_cam_yuva.solver = 'EXACT'
bool_cam_yuva.object = cam_yuva_kesici
bpy.context.view_layer.objects.active = govde
bpy.ops.object.modifier_apply(modifier="Cam_Oturma_Yuvasi")
bpy.data.objects.remove(cam_yuva_kesici, do_unlink=True)

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

# ==========================================
# 2. CEKMECE YUVASI VE CEKMECE
# ==========================================
yuva_w = 0.120  
yuva_d = 0.130  
yuva_h = 0.045  
yuva_z = 0.016 + (yuva_h / 2.0)

kesici_boy_y = yuva_d + 0.060
kesici_pos_y = - (yuva_d / 2.0) - 0.020

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, kesici_pos_y, yuva_z))
yuva_kesici = bpy.context.active_object
yuva_kesici.scale = (yuva_w, kesici_boy_y, yuva_h)
bpy.ops.object.transform_apply(scale=True)

bool_yuva = govde.modifiers.new(name="Cekmece_Yuvasi", type='BOOLEAN')
bool_yuva.operation = 'DIFFERENCE'
bool_yuva.solver = 'EXACT'
bool_yuva.object = yuva_kesici
bpy.context.view_layer.objects.active = govde
bpy.ops.object.modifier_apply(modifier="Cekmece_Yuvasi")
bpy.data.objects.remove(yuva_kesici, do_unlink=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, -0.3, yuva_z))
cekmece = bpy.context.active_object
cekmece.scale = (yuva_w - 0.001, yuva_d - 0.001, yuva_h - 0.001)
cekmece.name = "SmartPlanter_Cekmece"
bpy.ops.object.transform_apply(scale=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, -0.3, yuva_z + 0.003))
cekmece_ici = bpy.context.active_object
cekmece_ici.scale = (yuva_w - 0.008, yuva_d - 0.008, yuva_h - 0.005)
bpy.ops.object.transform_apply(scale=True)

bool_cekmece_ic = cekmece.modifiers.new(name="Cekmece_Ici", type='BOOLEAN')
bool_cekmece_ic.operation = 'DIFFERENCE'
bool_cekmece_ic.solver = 'EXACT'
bool_cekmece_ic.object = cekmece_ici
bpy.context.view_layer.objects.active = cekmece
bpy.ops.object.modifier_apply(modifier="Cekmece_Ici")
bpy.data.objects.remove(cekmece_ici, do_unlink=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, -0.3 - (yuva_d / 2.0) - 0.003, yuva_z))
kulp = bpy.context.active_object
kulp.scale = (0.040, 0.006, 0.012)
bpy.ops.object.transform_apply(scale=True)

bool_kulp = cekmece.modifiers.new(name="Kulp_Birlestir", type='BOOLEAN')
bool_kulp.operation = 'UNION'
bool_kulp.solver = 'EXACT'
bool_kulp.object = kulp
bpy.context.view_layer.objects.active = cekmece
bpy.ops.object.modifier_apply(modifier="Kulp_Birlestir")
bpy.data.objects.remove(kulp, do_unlink=True)

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0, 0.115, 0.035))
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

# ==========================================
# 3. SAKSI ALTLIGI (24.5 CM IC HAVUZLU KARE TABAK)
# ==========================================
altlik_ic_cap = 0.245             
altlik_ic_yaricap = altlik_ic_cap / 2.0  
altlik_et_kalinligi = 0.004       
altlik_derinlik = 0.010           

kare_kenar = (altlik_ic_yaricap + altlik_et_kalinligi) * 2.0  
altlik_toplam_yukseklik = altlik_derinlik + altlik_et_kalinligi  

altlik_pos_x = 0.35
altlik_merkez_z = altlik_toplam_yukseklik / 2.0

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(altlik_pos_x, 0, altlik_merkez_z))
altlik = bpy.context.active_object
altlik.name = "SmartPlanter_KareAltlik"
altlik.scale = (kare_kenar, kare_kenar, altlik_toplam_yukseklik)
bpy.ops.object.transform_apply(scale=True)

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

# ==========================================
# 4. ENTEGRE GECME YUVALI LED KAPAGI
# ==========================================
kapak_dis_cap = 0.238
kapak_dis_yaricap = kapak_dis_cap / 2.0
kapak_toplam_h = 0.040
kapak_pos_x = -0.35
kapak_merkez_z = kapak_toplam_h / 2.0

bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=kapak_dis_yaricap,
    depth=kapak_toplam_h,
    location=(kapak_pos_x, 0, kapak_merkez_z)
)
kapak = bpy.context.active_object
kapak.name = "SmartPlanter_LedKapak"

cam_gecme_yaricap = 0.102  
cam_gecme_derinlik = 0.015
cam_gecme_z = (cam_gecme_derinlik / 2.0) - 0.001

bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=cam_gecme_yaricap,
    depth=cam_gecme_derinlik + 0.002,
    location=(kapak_pos_x, 0, cam_gecme_z)
)
cam_gecme_kesici = bpy.context.active_object

bool_gecme = kapak.modifiers.new(name="Cam_Gecme_Yuvasi", type='BOOLEAN')
bool_gecme.operation = 'DIFFERENCE'
bool_gecme.solver = 'EXACT'
bool_gecme.object = cam_gecme_kesici
bpy.context.view_layer.objects.active = kapak
bpy.ops.object.modifier_apply(modifier="Cam_Gecme_Yuvasi")
bpy.data.objects.remove(cam_gecme_kesici, do_unlink=True)

merkez_baca_yaricap = 0.085
bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=merkez_baca_yaricap,
    depth=kapak_toplam_h + 0.004,
    location=(kapak_pos_x, 0, kapak_merkez_z)
)
baca_kesici = bpy.context.active_object

bool_baca = kapak.modifiers.new(name="Merkez_Baca_Acikligi", type='BOOLEAN')
bool_baca.operation = 'DIFFERENCE'
bool_baca.solver = 'EXACT'
bool_baca.object = baca_kesici
bpy.context.view_layer.objects.active = kapak
bpy.ops.object.modifier_apply(modifier="Merkez_Baca_Acikligi")
bpy.data.objects.remove(baca_kesici, do_unlink=True)

led_kanal_yaricap = 0.098
led_kanal_derinlik = 0.008
led_kanal_z = (kapak_toplam_h - 0.010)

bpy.ops.mesh.primitive_cylinder_add(
    vertices=128,
    radius=led_kanal_yaricap,
    depth=led_kanal_derinlik,
    location=(kapak_pos_x, 0, led_kanal_z)
)
led_kesici = bpy.context.active_object

bool_led = kapak.modifiers.new(name="LED_Kanali", type='BOOLEAN')
bool_led.operation = 'DIFFERENCE'
bool_led.solver = 'EXACT'
bool_led.object = led_kesici
bpy.context.view_layer.objects.active = kapak
bpy.ops.object.modifier_apply(modifier="LED_Kanali")
bpy.data.objects.remove(led_kesici, do_unlink=True)

bpy.ops.mesh.primitive_cylinder_add(
    vertices=16,
    radius=0.0025,
    depth=0.040,
    location=(kapak_pos_x + kapak_dis_yaricap - 0.01, 0, kapak_merkez_z),
    rotation=(0, 1.5708, 0)
)
kablo_deligi = bpy.context.active_object

bool_kablo = kapak.modifiers.new(name="Kablo_Deligi", type='BOOLEAN')
bool_kablo.operation = 'DIFFERENCE'
bool_kablo.solver = 'EXACT'
bool_kablo.object = kablo_deligi
bpy.context.view_layer.objects.active = kapak
bpy.ops.object.modifier_apply(modifier="Kablo_Deligi")
bpy.data.objects.remove(kablo_deligi, do_unlink=True)

apply_smooth(kapak, 30.0)

# ==========================================
# 5. UÇLARI AÇIK YAYLI MANDALLAR (4 ADET)
# ==========================================
def create_leaf_spring_clip(name, loc_x, loc_y, loc_z):
    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x, loc_y, loc_z))
    mandal = bpy.context.active_object
    mandal.name = name
    mandal.scale = (0.018, 0.022, 0.050)
    bpy.ops.object.transform_apply(scale=True)

    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x - 0.006, loc_y, loc_z))
    cam_kanali = bpy.context.active_object
    cam_kanali.scale = (0.010, 0.024, 0.034)
    bpy.ops.object.transform_apply(scale=True)

    bool_cam = mandal.modifiers.new(name="Cam_Kanali", type='BOOLEAN')
    bool_cam.operation = 'DIFFERENCE'
    bool_cam.solver = 'EXACT'
    bool_cam.object = cam_kanali
    bpy.context.view_layer.objects.active = mandal
    bpy.ops.object.modifier_apply(modifier="Cam_Kanali")
    bpy.data.objects.remove(cam_kanali, do_unlink=True)

    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x + 0.009, loc_y, loc_z + 0.022))
    ust_tirnak = bpy.context.active_object
    ust_tirnak.scale = (0.004, 0.020, 0.006)
    bpy.ops.object.transform_apply(scale=True)

    bool_ust = mandal.modifiers.new(name="Ust_Tirnak", type='BOOLEAN')
    bool_ust.operation = 'UNION'
    bool_ust.solver = 'EXACT'
    bool_ust.object = ust_tirnak
    bpy.context.view_layer.objects.active = mandal
    bpy.ops.object.modifier_apply(modifier="Ust_Tirnak")
    bpy.data.objects.remove(ust_tirnak, do_unlink=True)

    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x + 0.009, loc_y, loc_z - 0.022))
    alt_tirnak = bpy.context.active_object
    alt_tirnak.scale = (0.004, 0.020, 0.006)
    bpy.ops.object.transform_apply(scale=True)

    bool_alt = mandal.modifiers.new(name="Alt_Tirnak", type='BOOLEAN')
    bool_alt.operation = 'UNION'
    bool_alt.solver = 'EXACT'
    bool_alt.object = alt_tirnak
    bpy.context.view_layer.objects.active = mandal
    bpy.ops.object.modifier_apply(modifier="Alt_Tirnak")
    bpy.data.objects.remove(alt_tirnak, do_unlink=True)

    # Uçları açık esnek yay kolu
    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x - 0.010, loc_y, loc_z), rotation=(0, 0.12, 0))
    yay_kolu = bpy.context.active_object
    yay_kolu.scale = (0.0035, 0.022, 0.042)
    bpy.ops.object.transform_apply(scale=True, rotation=True)

    bool_yay = mandal.modifiers.new(name="Yay_Kolu", type='BOOLEAN')
    bool_yay.operation = 'UNION'
    bool_yay.solver = 'EXACT'
    bool_yay.object = yay_kolu
    bpy.context.view_layer.objects.active = mandal
    bpy.ops.object.modifier_apply(modifier="Yay_Kolu")
    bpy.data.objects.remove(yay_kolu, do_unlink=True)

    # Orta bağlantı köprüsü
    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(loc_x - 0.005, loc_y, loc_z))
    kopru = bpy.context.active_object
    kopru.scale = (0.007, 0.022, 0.008)
    bpy.ops.object.transform_apply(scale=True)

    bool_kopru = mandal.modifiers.new(name="Orta_Kopru", type='BOOLEAN')
    bool_kopru.operation = 'UNION'
    bool_kopru.solver = 'EXACT'
    bool_kopru.object = kopru
    bpy.context.view_layer.objects.active = mandal
    bpy.ops.object.modifier_apply(modifier="Orta_Kopru")
    bpy.data.objects.remove(kopru, do_unlink=True)

    apply_smooth(mandal, 30.0)

mandal_merkez_z = 0.050 / 2.0
aralik = 0.035

for i in range(4):
    pos_y = (i - 1.5) * aralik
    create_leaf_spring_clip(f"SmartPlanter_Mandal_{i+1}", 0, 0.35 + pos_y, mandal_merkez_z)

print("Uclari acik, orta baglantili yayli mandallar basariyla olusturuldu!")
