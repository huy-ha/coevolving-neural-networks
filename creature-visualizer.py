from bpy.types import (Operator, Panel, AddonPreferences)
from bpy.props import (BoolProperty, StringProperty)
import bpy
import json
import bmesh
from mathutils import Vector
from math import radians

bl_info = {
    "name": "CreatureVisualizer",
    "author": "Huy Ha <huy.ha@columbia.edu>",
    "version": (0, 0, 1),
    "blender": (2, 80, 0),
    "location": "3D View > Toolbox > Animation tab > Creature Visualizer",
    "description": "Imports Soft Box Creature Simulation Result",
    "warning": "",
    "category": "Animation",
}


class VIEW3D_PT_creature_viz(Panel):
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "Animation"
    bl_label = 'CreatureViz'

    def draw(self, context):
        obj = context.active_object

        layout = self.layout
        col = layout.column(align=True)
        col.prop(context.scene, 'creature_path')
        row = layout.row()
        row.operator("creatureviz.import")


class ANIM_OT_import_creature(Operator):
    bl_label = "Import"
    bl_idname = "creatureviz.import"
    bl_description = "Imports a creature"
    bl_options = {'REGISTER', 'UNDO'}

    def invoke(self, context, event):
        self.execute(context)

        return {'FINISHED'}

    def execute(op, context):
        obj = context.active_object
        with open(context.scene.creature_path) as json_file:
            data = json.load(json_file)
            name = data["name"]

            vertices = []
            edges = []

            center = data["center"]

            for vertex in data["mesh"]["vertices"]:
                vertices.append((vertex['x'] - center["x"],
                                 vertex['y'] - center["y"], vertex['z'] - center["z"]))

            mesh = bpy.data.meshes.new(name="{} Mesh".format(name))
            mesh.from_pydata(vertices, edges, data["mesh"]["faces"])
            mesh.update()
            mesh.validate()

            obj = bpy.data.objects.new(name, mesh)
            scene = bpy.context.scene
            scene.collection.objects.link(obj)
            bpy.ops.object.select_all(action='DESELECT')
            obj.select_set(state=True)
            context.view_layer.objects.active = obj
            print("\nCREATURE VIZ:{}".format(str(context.area.type)))

            # Animate
            bpy.context.window_manager.animall_properties.key_points = True

            mesh = obj.data
            vertices = mesh.vertices
            frame_ctr = 0
            bpy.ops.object.mode_set(mode='OBJECT')
            for frame in data["simulation"]:
                bm = bmesh.new()
                bm.from_mesh(mesh)

                bpy.context.scene.frame_set(frame_ctr)
                pos = frame["positions"]

                for i, v in enumerate(bm.verts):
                    v.co = Vector((pos[3*i], pos[3*i + 1], pos[3*i + 2]))
                # Finish up, write the bmesh back to the mesh
                bm.to_mesh(mesh)
                bm.free()  # free and prevent further access
                bpy.ops.object.mode_set(mode='EDIT')
                bpy.ops.mesh.select_all(action='SELECT')
                bpy.ops.anim.insert_keyframe_animall()
                bpy.ops.object.mode_set(mode='OBJECT')
                frame_ctr += 1
            obj.rotation_euler = (radians(90), 0, 0)


def register():
    bpy.utils.register_class(VIEW3D_PT_creature_viz)
    bpy.types.Scene.creature_path = bpy.props.StringProperty(
        name="Creature Path",
        default="",
        description="Define the path to the creature's json file",
        subtype='FILE_PATH'
    )
    bpy.utils.register_class(ANIM_OT_import_creature)


def unregister():
    bpy.utils.unregister_class(VIEW3D_PT_creature_viz)
    del bpy.types.Scene.creature_path
    bpy.utils.unregister_class(ANIM_OT_import_creature)


if __name__ == "__main__":
    register()
